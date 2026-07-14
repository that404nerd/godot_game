#include "weapon_manager.h"

void WeaponManager::_ready()
{
  AnimationPlayer* anim_player = nullptr;
  Node3D* weapon_node = nullptr;
  WeaponWrapper* weapon_wrapper = nullptr;

  _init_weapons();
  _init_weapon_anim_connections(weapon_node, weapon_wrapper, anim_player);

  // Init the weapon wrapper instance nodes
  m_WeaponWrapperInst = m_WeaponNodes[m_WeaponIndex]->get_node<WeaponWrapper>(NodePath("WeaponWrapper"));
  m_MuzzleComp = m_WeaponWrapperInst->get_muzzle_flash_component();
  m_CurrentWeaponAnimPlayer = m_WeaponWrapperInst->get_weapon_anim_player();
  m_Skeleton3D = m_WeaponWrapperInst->get_armature_skeleton();

  m_CharacterBody = character_component;

  m_Camera = get_node<Camera3D>(NodePath("%PlayerCamera"));
  m_Viewport = get_viewport();
  m_ScreenCenter = m_Viewport->get_visible_rect().get_size() / 2.0f;

  m_Viewport->connect("size_changed", Callable(this, "_on_window_size_changed"));

  m_AmmoComp._init_data(weapon_component->get_weapon_resource_list());

  m_CurrentWeaponAnimPlayer = m_WeaponWrapperInst->get_weapon_anim_player();
  
  _change_fov(weapon_node, weapon_wrapper);

  weapon_component->set_current_weapon(weapon_component->get_weapon_resource_list()[m_WeaponIndex]);
  m_CurrentWeapon = weapon_component->get_current_weapon_data();
  m_DecalScene = m_CurrentWeapon->get_weaponDecalResource();
  m_RecoilResource = m_CurrentWeapon->get_weaponRecoilPatternResource();

  Node* pathNode = m_RecoilResource->instantiate();
  Path2D* path = Object::cast_to<Path2D>(pathNode);

  m_RecoilCurve = path->get_curve();

  m_WeaponEffects._init_data({ 
    .HoldPointNode = hold_point_node,
    .MovementManagerInst = movement_manager,
    .CharacterCompInst = character_component,
    .WeaponCompInst = weapon_component });
}

void WeaponManager::_init_weapons()
{
  for(int i = 0; i < weapon_component->get_weapon_resource_list().size(); i++)
  {
    weapon_component->set_current_weapon(weapon_component->get_weapon_resource_list()[i]);
    m_CurrentWeapon = weapon_component->get_current_weapon_data();

    Ref<PackedScene> packedScene = m_CurrentWeapon->get_weaponScene();
    hold_point_node->add_child(packedScene->instantiate());
  }
}

void WeaponManager::_init_weapon_anim_connections(Node3D* weapon_node, WeaponWrapper* weapon_wrapper, AnimationPlayer* anim_player)
{
  /* This took me 2 hours to find lol, i forgot that i was dealing with different animation
      players for different weapon scene, this connects the _on_animation_finished to all animation players */
  for(int i = 0; i < hold_point_node->get_children().size(); i++)
  {
    m_WeaponNodes.push_back(Object::cast_to<Node3D>(hold_point_node->get_children()[i]));
    weapon_node = Object::cast_to<Node3D>(m_WeaponNodes[i]);
    weapon_wrapper = weapon_node->get_node<WeaponWrapper>(NodePath("WeaponWrapper"));

    weapon_node->set_visible(false);

    m_WeaponAnims.push_back(weapon_wrapper->get_weapon_anim_player());
    anim_player = Object::cast_to<AnimationPlayer>(m_WeaponAnims[i]);

    anim_player->connect("animation_started", Callable(this, "_on_weapon_anim_started"));

    /* I have seperate functions in both the weapon state machine and this class that connect to the same signal
    but the state machine's animation finished function only handles the state part only! */
    anim_player->connect("animation_finished", Callable(weapon_state_machine, "_on_animation_finished"));
    anim_player->connect("animation_finished", Callable(this, "_on_weapon_anim_finished"));
  }
}

void WeaponManager::_change_fov(Node3D* weapon_node, WeaponWrapper* weapon_wrapper)
{
  for(int weaponCount = 0; weaponCount < hold_point_node->get_children().size(); weaponCount++)
  { 
    m_WeaponNodes.push_back(Object::cast_to<Node3D>(hold_point_node->get_children()[weaponCount]));
    weapon_node = Object::cast_to<Node3D>(m_WeaponNodes[weaponCount]);
    weapon_wrapper = weapon_node->get_node<WeaponWrapper>(NodePath("WeaponWrapper"));

    weapon_component->set_current_weapon(weapon_component->get_weapon_resource_list()[weaponCount]);
    m_CurrentWeapon = weapon_component->get_current_weapon_data();

    
    for(int meshes = 0; meshes < weapon_wrapper->get_mesh_instances().size(); meshes++)
    {
      NodePath meshNodePath = weapon_wrapper->get_mesh_instances()[meshes];
      MeshInstance3D* mesh = weapon_wrapper->get_node<MeshInstance3D>(meshNodePath);

      /*
        NOTE: This is for future me, just in case. 

        Godot basically has two ways of setting materials. Surface overrides materials and just material overrides. 
        By default every mesh has 1 empty surface override materials, this is for setting materials for individual meshes and it depends on how the model is imported and structured.
        Then there's the Material Overrides, which is a single material that is applied to the entire mesh instead of individual parts/meshes.

      */
      if(mesh->get_surface_override_material_count() > 1)
      {
        /*
          If the mesh has surface override materials then loop through all the available ones.
        */
        for(int surfaceMaterials = 0; surfaceMaterials < mesh->get_surface_override_material_count(); surfaceMaterials++)
        {
          Ref<Material> material = mesh->get_active_material(surfaceMaterials);
         
          /*
            First we set the existing material to the mesh before we hold another reference to material (References basically).
            Also, just in case. If a material is shared between two meshes then setting the override later on will affect the material of the other mesh since both hold a reference to the same object.
            I have unique meshes so i didn't duplicate it before setting the m_StdMaterial to the duplicated material and then setting the fov_override. 
          */
          mesh->set_surface_override_material(surfaceMaterials, material);
          
          m_StdMaterial = material;
          if (m_StdMaterial.is_valid()) {
            m_StdMaterial->set_fov_override(m_CurrentWeapon->get_weaponFOV());
          }
        }
      }
      else {
        // Here, just get the one material override (p_surface is 0 for these types of meshes).
        Ref<Material> material = mesh->get_material_override();
        mesh->set_surface_override_material(0, material);
        
        m_StdMaterial = material;
        if(m_StdMaterial.is_valid())
          m_StdMaterial->set_fov_override(m_CurrentWeapon->get_weaponFOV());
      }
    }
  }
}

void WeaponManager::_on_window_size_changed()
{
  m_ScreenCenter = m_Viewport->get_visible_rect().get_size() / 2.0f;
}

void WeaponManager::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_weapon_anim_started", "anim_name"), &WeaponManager::_on_weapon_anim_started);
  ClassDB::bind_method(D_METHOD("_on_weapon_anim_finished", "anim_name"), &WeaponManager::_on_weapon_anim_finished);
  ClassDB::bind_method(D_METHOD("_on_window_size_changed"), &WeaponManager::_on_window_size_changed);
  
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, movement_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, hold_point_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ClassDB::bind_method(D_METHOD("get_current_weapon_ammo"), &WeaponManager::get_current_weapon_ammo);
  ClassDB::bind_method(D_METHOD("get_current_reserve_ammo"), &WeaponManager::get_current_reserve_ammo);
  ClassDB::bind_method(D_METHOD("get_current_weapon_name"), &WeaponManager::get_current_weapon_name);
}

void WeaponManager::_unhandled_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouseEvent = event;
  
  if(event->is_class("InputEventMouseMotion")) {
    float swayIntensity = 0.005f; 

    Vector2 relative = mouseEvent->get_relative(); 
    m_MouseVel.x += -relative.x * swayIntensity;
    m_MouseVel.y += -relative.y * swayIntensity;
  }
}

void WeaponManager::_process(double delta)
{
  m_CurrentWeapon = weapon_component->get_current_weapon_data();
  m_WeaponStateCtx.CurrentWeaponType = m_CurrentWeapon->get_weapon_type();
  
  m_HoldMaxTime = m_CurrentWeapon->get_hold_max_time();
  
  m_TimeBetweenShots -= delta;

  m_WeaponEffects._update(delta, m_MouseVel);
}

void WeaponManager::_physics_process(double delta)
{
  // Get space state in _process or some other function could cause errors because
  // for some reason it's not accessible unless it's in the _physics_process

  Vector3 ray_start = m_Camera->project_ray_origin(m_ScreenCenter);
  Vector3 ray_end = ray_start + m_Camera->project_ray_normal(m_ScreenCenter) * 1000.0f;

  m_SpaceState = m_CharacterBody->get_world_3d()->get_direct_space_state();
  m_Query = PhysicsRayQueryParameters3D::create(ray_start, ray_end);

  m_Query->set_collide_with_bodies(true);

  m_Result = m_SpaceState->intersect_ray(m_Query);
}

// Runs when you switch a weapon (Only once)
void WeaponManager::_update_weapon_data(Ref<Weapon> nextWeapon)
{
  m_WeaponWrapperInst = m_WeaponNodes[m_WeaponIndex]->get_node<WeaponWrapper>(NodePath("WeaponWrapper"));
  m_MuzzleComp = m_WeaponWrapperInst->get_muzzle_flash_component();
  m_CurrentWeaponAnimPlayer = m_WeaponWrapperInst->get_weapon_anim_player();
  m_Skeleton3D = m_WeaponWrapperInst->get_armature_skeleton();

  m_DecalScene = nextWeapon->get_weaponDecalResource();
  m_RecoilResource = nextWeapon->get_weaponRecoilPatternResource();

  Node* pathNode = m_RecoilResource->instantiate();
  Path2D* path = Object::cast_to<Path2D>(pathNode);

  m_RecoilCurve = path->get_curve();
}

void WeaponManager::generate_decal()
{
  // for(int i = 0; i < m_CurrentWeapon->get_noOfProjectilesAtSameTime(); i++)
  {
    if(!m_Result.is_empty())
    {
      Node* instance = m_DecalScene->instantiate();
      Decal* bulletDecal = Object::cast_to<Decal>(instance);

      // CollisionObject3D* colliderBody = Object::cast_to<CollisionObject3D>(m_Result["collider"]);

      add_child(bulletDecal);
      Vector3 position = Vector3(m_Result["position"]);
      bulletDecal->set_global_position(position);
      bulletDecal->look_at(bulletDecal->get_global_transform().origin + m_Result["normal"], Vector3(0.0f, 1.0f, 0.0f));
      bulletDecal->rotate_object_local(Vector3(1.0f, 0.0f, 0.0f), 90.0f);
    }
  }
}

void WeaponManager::_on_weapon_anim_started(const StringName& anim_name)
{
  if(anim_name == StringName(m_CurrentWeapon->get_weaponShootingAnimName()))
  {
    m_MuzzleComp->_set_particles_status(true);
    m_MuzzleLightTimeout = m_MuzzleComp->get_particle_lifetime();

    m_AmmoComp.consume_ammo(m_CurrentWeapon, 1);
    generate_decal();
    EventBus::get_singleton()->emit_signal("weapon_fired", m_RecoilCurve);
  }

  if(anim_name == StringName(m_CurrentWeapon->get_weaponReloadAnimName()))
  {
    EventBus::get_singleton()->emit_signal("weapon_reload_start", m_Skeleton3D);
  }
}

void WeaponManager::_on_weapon_anim_finished(const StringName& anim_name)
{
  // Make sure the reload state is over for any type of reload once the animation ends
  if(anim_name == StringName(m_CurrentWeapon->get_weaponReloadAnimName()))
  {
    m_WeaponStateCtx.IsReloading = false;
  }

  if(anim_name == StringName(m_CurrentWeapon->get_weaponUnequipAnimName()))
  {
    m_WeaponStateCtx.IsUnequipped = true;
  }

  if(anim_name == StringName(m_CurrentWeapon->get_weaponShootingAnimName()))
  {
    m_MuzzleComp->_set_particles_status(false);
  }

  ////////////////////////////////////////////////////////////////
  //////////////////// For incremental reloads ///////////////////
  ////////////////////////////////////////////////////////////////

  int current_ammo = m_AmmoComp.get_current_weapon_ammo(m_CurrentWeapon); // ammo that's currently in the magazine
  int current_reserve_ammo = m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon); // reserve ammo
  int max_mag_capacity = m_CurrentWeapon->get_magAmmoCount(); // total capacity of the magazine (read only)
  int ammoNeeded = max_mag_capacity - current_ammo;
  int ammoToBeReloaded = Math::min(ammoNeeded, current_reserve_ammo);

  // make sure this only triggers for weapons with incremental reloads only!!!!!
  if(m_CurrentWeapon->get_is_incremental_reload())
  {
    if(anim_name == StringName(m_CurrentWeapon->get_weaponReloadStartAnimName()))
    {
      m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadAnimName(),
        m_CurrentWeapon->get_weapon_reload_anim_blend(), m_CurrentWeapon->get_weapon_reload_anim_speed());
    }

    if(anim_name == StringName(m_CurrentWeapon->get_weaponReloadAnimName()))
    {
      m_AmmoComp.set_current_weapon_ammo(m_CurrentWeapon, current_ammo + 1);
      m_AmmoComp.set_current_weapon_reserve_ammo(m_CurrentWeapon, current_reserve_ammo - 1);
      
      current_ammo = m_AmmoComp.get_current_weapon_ammo(m_CurrentWeapon); // ammo that's currently in the magazine
      current_reserve_ammo = m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon); // reserve ammo
      ammoNeeded = max_mag_capacity - current_ammo;
      ammoToBeReloaded = Math::min(ammoNeeded, current_reserve_ammo);


      if(ammoToBeReloaded == 0)
      {
        m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadEndAnimName(),
          m_CurrentWeapon->get_weapon_reload_end_anim_blend(), m_CurrentWeapon->get_weapon_reload_end_anim_speed());

        m_WeaponStateCtx.IsReloading = false;
        m_WeaponStateCtx.IsReloadStarted = false;
      }
      
      if(ammoToBeReloaded > 0)
      {
        m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadAnimName(),
          m_CurrentWeapon->get_weapon_reload_anim_blend(), m_CurrentWeapon->get_weapon_reload_anim_speed());
      }
      
    }

  }
  

}

///////////////////////////////////////////////////////////////////////
/////////////////// Weapon State Implementation ///////////////////////
///////////////////////////////////////////////////////////////////////
void WeaponManager::_equip_weapon()
{
  m_WeaponStateCtx.IsUnequipped = false;
  m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName(), 
                            m_CurrentWeapon->get_weapon_equip_anim_blend(), m_CurrentWeapon->get_weapon_equip_anim_speed());
}

void WeaponManager::_unequip_weapon()
{
  if(weapon_component->get_next_weapon_name() != m_CurrentWeapon->get_weaponName())
  {
    if(m_CurrentWeaponAnimPlayer->get_current_animation() != m_CurrentWeapon->get_weaponUnequipAnimName())
    {
      m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponUnequipAnimName(), 
        m_CurrentWeapon->get_weapon_unequip_anim_blend(), m_CurrentWeapon->get_weapon_unequip_anim_speed());
    } 
  } else m_WeaponStateCtx.IsUnequipped = true;
}



void WeaponManager::_shoot_weapon(double delta)
{
  // Don't even shoot, just switch to the idle state instead
  if(m_AmmoComp.is_ammo_empty(m_CurrentWeapon) ||
    (m_AmmoComp.is_ammo_empty(m_CurrentWeapon) && m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon) == 0))
  {
    m_MuzzleComp->_enable_light_status(false);
    m_WeaponStateCtx.ShootTimeBeforeIdle = 0.0f;
    m_WeaponStateCtx.IsKeyHeld = false;
    m_WeaponStateCtx.IsKeyPressed = false;
    return;
  }

  m_WeaponStateCtx.IsWeaponFiring = true;

  m_ShootTimer += delta;
  m_ShootTimer = Math::clamp(m_ShootTimer, 0.0f, 1.0f);

  // Start the timer (which gives a grace period before switching to idle state of the weapon) if it's less than or equal to 0.0f
  if(m_WeaponStateCtx.ShootTimeBeforeIdle >= 0.0f)
  {
    m_WeaponStateCtx.ShootTimeBeforeIdle -= delta;
  }
  
  if(m_TimeBetweenShots <= 0.0f)
  {
    // Check whether the fire key is held or not (for automatic weapons)
    if(Input::get_singleton()->is_action_pressed("shoot_weapon") && (
          m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::AUTO || m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::BOTH)) 
    {
      m_HoldCounter += delta;
      
      if(m_HoldCounter > m_HoldMaxTime)
      {
        m_WeaponStateCtx.IsKeyHeld = true;
      }
      
      if(m_WeaponStateCtx.IsKeyHeld)
      {
        m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName(), 
          m_CurrentWeapon->get_weapon_shoot_anim_blend(), m_CurrentWeapon->get_weapon_shoot_anim_speed());
      }

    }  

    // Check whether we pressed the fire key (manual)
    if(Input::get_singleton()->is_action_just_pressed("shoot_weapon") && 
      (m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::MANUAL || m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::BOTH))
    {
      m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName(), 
          m_CurrentWeapon->get_weapon_shoot_anim_blend(), m_CurrentWeapon->get_weapon_shoot_anim_speed());


      m_WeaponStateCtx.IsKeyPressed = true;
    }

    if(m_WeaponStateCtx.IsKeyHeld || m_WeaponStateCtx.IsKeyPressed)
    {
      m_MuzzleComp->_enable_light_status(true);
      m_WeaponStateCtx.ShootTimeBeforeIdle = 1.0f;

      m_TimeBetweenShots = m_CurrentWeapon->get_time_between_shots();
      m_WeaponStateCtx.IsKeyPressed = false;
    }

  }

  m_MuzzleLightTimeout -= delta;
  if(m_MuzzleLightTimeout >= 0.0f)
  {
    m_MuzzleComp->_enable_light_status(true);
  }

  if(m_MuzzleLightTimeout <= 0.0f)
    m_MuzzleComp->_enable_light_status(false);

  // Set the key held to false and reset the hold counter 
  if(Input::get_singleton()->is_action_just_released("shoot_weapon")) 
  {
    m_WeaponStateCtx.IsWeaponFiring = false;
    m_WeaponStateCtx.IsKeyHeld = false;
    m_HoldCounter = 0.0f;
  }
}

void WeaponManager::_shoot_weapon_over()
{
  m_ShootTimer = 0.0f;
  m_WeaponStateCtx.IsWeaponFiring = false;
  m_MuzzleComp->_enable_light_status(false);
}
  
void WeaponManager::_reload_weapon()
{

  int current_ammo = m_AmmoComp.get_current_weapon_ammo(m_CurrentWeapon); // ammo that's currently in the magazine
  int current_reserve_ammo = m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon); // reserve ammo
  int max_mag_capacity = m_CurrentWeapon->get_magAmmoCount(); // total capacity of the magazine (read only)
  
  if(current_ammo >= max_mag_capacity || current_reserve_ammo <= 0)
    return;

  int ammoNeeded = max_mag_capacity - current_ammo;
  int ammoToBeReloaded = Math::min(ammoNeeded, current_reserve_ammo);

  
  m_WeaponStateCtx.IsReloading = true;
  
  if(m_CurrentWeapon->get_is_incremental_reload())
  {
    // If we shoot mid-reload just cancel the entire reload
    if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
    {
      m_WeaponStateCtx.IsReloading = false;
      m_WeaponStateCtx.IsReloadStarted = false;
      return;
    }

    if(m_WeaponStateCtx.IsReloadStarted == false)
    {
      m_WeaponStateCtx.IsReloadStarted = true;
      m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadStartAnimName(),
      m_CurrentWeapon->get_weapon_reload_start_anim_blend(), m_CurrentWeapon->get_weapon_reload_start_anim_speed());
    }

  } else {
    m_CurrentWeaponAnimPlayer->play(
      m_CurrentWeapon->get_weaponReloadAnimName(), 
      m_CurrentWeapon->get_weapon_reload_anim_blend(), 
      m_CurrentWeapon->get_weapon_reload_anim_speed()
    );

    // This will work for weapons that are not using incremental reloads
    // The ammo won't be set if you change the weapon before the mag is entered
    if(get_current_anim_length() >= m_CurrentWeapon->get_magEnteredTimestamp())
    {
      m_AmmoComp.set_current_weapon_ammo(m_CurrentWeapon, current_ammo + ammoToBeReloaded);
      m_AmmoComp.set_current_weapon_reserve_ammo(m_CurrentWeapon, current_reserve_ammo - ammoToBeReloaded);
    }
    
  }
}

void WeaponManager::_weapon_unequip_over()
{
  m_CurrentWeaponAnimPlayer = m_WeaponAnims[m_WeaponIndex];
  weapon_component->set_current_weapon(weapon_component->get_next_weapon_data());
}


void WeaponManager::_weapon_switch()
{
  for (int i = 0; i < weapon_component->get_weapon_resource_list().size(); i++) {
    Ref<Weapon> res = weapon_component->get_weapon_resource_list()[i]; 

    if (res.is_valid() && res->get_weaponName() == weapon_component->get_next_weapon_name()) {
      m_WeaponIndex = i;
      break;
    }
  }
  
  if(m_WeaponIndex != -1)
  {
    Ref<Weapon> nextWeapon = weapon_component->get_weapon_resource_list()[m_WeaponIndex];
    weapon_component->set_current_weapon(nextWeapon);
    m_WeaponEffects._update_data(nextWeapon);
    
    _update_weapon_data(nextWeapon);
  }
}


void WeaponManager::_switch_weapon_data(int weaponIndex)
{
  // We are still in the previous weapon and didn't switch to the next one yet
  Ref<Weapon> next_weapon = weapon_component->get_weapon_resource_list()[weaponIndex];
  
  weapon_component->set_next_weapon(next_weapon);
  weapon_component->set_next_weapon_name(next_weapon->get_weaponName());
  m_WeaponIndex = weaponIndex;
}