#include "weapon_manager.h"

void WeaponManager::_init()
{
  set_physics_process(false);
  set_process(false);
    
  ERR_FAIL_COND_MSG(!input_command_system, "[Weapon Manager]: Input Command System is null");
  ERR_FAIL_COND_MSG(!weapon_state_machine, "[Weapon Manager]: Weapon State Machine is null");
  ERR_FAIL_COND_MSG(!weapon_component, "[Weapon Manager]: Weapon Component is null");

  _init_weapons();
  _init_weapon_anim_connections();

  // Init the weapon wrapper instance nodes
  _init_weapon_manager_data();
  m_AmmoComp._init_data(weapon_component->get_weapon_res_list());

  print_line("Weapon Manager Initialized");
}

void WeaponManager::_init_weapons()
{
  Node3D* weaponNode = nullptr;
  Array weaponResList = weapon_component->get_weapon_res_list();
  Array weaponSceneList = weapon_component->get_weapon_scene_list();

  ERR_FAIL_COND_MSG(weaponResList.size() == 0, "No weapons in the weapon component");

  if(weapons_init_required)
  {
    for(int i = 0; i < weaponResList.size(); i++)
    {
      weapon_component->set_current_weapon_res(weaponResList[i]);
      m_CurrentWeapon = weapon_component->get_current_weapon_res();
      
      Ref<PackedScene> packedScene = weaponSceneList[i];
      weaponNode = Object::cast_to<Node3D>(packedScene->instantiate());

      // Hide all the weapons since the first weapon's equip anim will set the visible to true anyways.
      weaponNode->set_visible(false);

      m_WeaponNodes.push_back(weaponNode);
      hold_point_node->add_child(weaponNode);
    }

  } else {
    Array weapons = hold_point_node->get_children();
    for(int i = 0; i < weapons.size(); i++)
    {
      m_WeaponNodes.push_back(Object::cast_to<Node3D>(weapons[i]));
    }
  }
}

void WeaponManager::_init_weapon_anim_connections()
{
  EventBus::get_singleton()->connect("anim_started", Callable(this, "_on_weapon_anim_started"));
    
  /* I have seperate functions in both the weapon state machine and this class that connect to the same signal
  but the state machine's animation finished function only handles the state part only! */
  EventBus::get_singleton()->connect("anim_finished", Callable(weapon_state_machine, "_on_animation_finished"));
  EventBus::get_singleton()->connect("anim_finished", Callable(this, "_on_weapon_anim_finished"));
}

void WeaponManager::_init_weapon_manager_data()
{
  Node3D* weapon_node = nullptr;
  WeaponWrapper* weapon_wrapper = nullptr;

  m_WeaponWrapperInst = m_WeaponNodes[m_WeaponIndex]->get_node<WeaponWrapper>(NodePath("WeaponWrapper"));
  
  ERR_FAIL_COND_MSG(!m_WeaponWrapperInst, "[Weapon Manager]: Weapon Wrapper is null!");

  m_MuzzleComp = m_WeaponWrapperInst->get_muzzle_flash_component();
  m_WeaponMuzzleMarker = m_WeaponWrapperInst->get_muzzle_point_marker();
  m_Skeleton3D = m_WeaponWrapperInst->get_armature_skeleton();

  if(animations_from_weapon)
    play_anim_component = m_WeaponWrapperInst->get_play_anim_component();

  // Make sure to change the fov before performing the rest of the initializations only if required
  if(weapon_fov_override_required)
    _change_fov();

  weapon_component->set_current_weapon_res(weapon_component->get_weapon_res_list()[m_WeaponIndex]);
  m_CurrentWeapon = weapon_component->get_current_weapon_res();
  m_DecalScene = m_CurrentWeapon->get_weaponDecalResource();
  m_CharacterBody = character_component;
}

void WeaponManager::_change_fov()
{
  Node3D* weapon_node = nullptr;
  WeaponWrapper* weapon_wrapper = nullptr;

  for(int weaponCount = 0; weaponCount < m_WeaponNodes.size(); weaponCount++)
  { 
    weapon_node = Object::cast_to<Node3D>(m_WeaponNodes[weaponCount]);
    weapon_wrapper = weapon_node->get_node<WeaponWrapper>(NodePath("WeaponWrapper"));

    weapon_component->set_current_weapon_res(weapon_component->get_weapon_res_list()[weaponCount]);
    m_CurrentWeapon = weapon_component->get_current_weapon_res();

    
    for(int meshes = 0; meshes < weapon_wrapper->get_mesh_instances().size(); meshes++)
    {
      NodePath meshNodePath = weapon_wrapper->get_mesh_instances()[meshes];
      MeshInstance3D* mesh = weapon_wrapper->get_node<MeshInstance3D>(meshNodePath);

      /*
        NOTE: This is for future me, just in case. 

        Godot basically has two ways of setting materials. Surface overrides materials and just material overrides. 
        By default every mesh has 1 empty surface override materials, this is for setting materials for 
        individual meshes and it depends on how the model is created.
        Then there's the Material Overrides, which is a single material that is applied to the entire mesh instead of individual parts/meshes.

      */
      if(mesh->get_surface_override_material_count() > 1)
      {
        /*
          If the mesh has surface override materials then loop through all the available ones.
        */
        for(int surfaceMatCount = 0; surfaceMatCount < mesh->get_surface_override_material_count(); surfaceMatCount++)
        {
          Ref<Material> material = mesh->get_active_material(surfaceMatCount);
          Ref<Material> dupMat = material->duplicate();
         
          /*
            First we set the existing material to the mesh before we hold another reference to material (References basically).

            Also, just in case. If a material is shared between two meshes then setting the override will affect the material
            of the other mesh since both hold a reference to the same object.

            I have unique meshes so i didn't duplicate it before setting the m_StdMaterial 
            to the duplicated material and then setting the fov_override. 
          */
          mesh->set_surface_override_material(surfaceMatCount, dupMat);
          
          m_StdMaterial = dupMat;
          if(m_StdMaterial.is_valid()) {
            m_StdMaterial->set_flag(BaseMaterial3D::FLAG_USE_Z_CLIP_SCALE, true);
            m_StdMaterial->set_z_clip_scale(m_CurrentWeapon->get_weaponZClipScale());
            m_StdMaterial->set_flag(BaseMaterial3D::FLAG_USE_FOV_OVERRIDE, true);
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
        {
          m_StdMaterial->set_flag(BaseMaterial3D::FLAG_USE_Z_CLIP_SCALE, true);
          m_StdMaterial->set_z_clip_scale(m_CurrentWeapon->get_weaponZClipScale());
          m_StdMaterial->set_flag(BaseMaterial3D::FLAG_USE_FOV_OVERRIDE, true);
          m_StdMaterial->set_fov_override(m_CurrentWeapon->get_weaponFOV());
        }
      }
    }
  }
}

void WeaponManager::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_weapon_anim_started", "anim_name"), &WeaponManager::_on_weapon_anim_started);
  ClassDB::bind_method(D_METHOD("_on_weapon_anim_finished", "anim_name"), &WeaponManager::_on_weapon_anim_finished);
 
  GD_BIND_PROPERTY(WeaponManager, weapons_init_required, Variant::BOOL);
  GD_BIND_PROPERTY(WeaponManager, weapon_fov_override_required, Variant::BOOL);
  GD_BIND_PROPERTY(WeaponManager, animations_from_weapon, Variant::BOOL);

  ADD_GROUP("Manager Nodes", "");
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, InputCommandSystem, input_command_system, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, WeaponStateMachine, weapon_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, WeaponComponent, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, CharacterComponent, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, PlayAnimComponent, play_anim_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, HealthComponent, health_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, Node3D, hold_point_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ClassDB::bind_method(D_METHOD("get_current_weapon_ammo"), &WeaponManager::get_current_weapon_ammo);
  ClassDB::bind_method(D_METHOD("get_current_reserve_ammo"), &WeaponManager::get_current_reserve_ammo);
  ClassDB::bind_method(D_METHOD("get_current_weapon_name"), &WeaponManager::get_current_weapon_name);
}

void WeaponManager::_unhandled_input(const Ref<InputEvent>& event)
{
}

void WeaponManager::_update(double delta)
{
  m_CurrentWeapon = weapon_component->get_current_weapon_res();
  m_WeaponStateCtx.CurrentWeaponType = m_CurrentWeapon->get_weapon_type();

  m_MuzzleComp->set_global_transform(m_WeaponMuzzleMarker->get_global_transform());

  m_HoldMaxTime = m_CurrentWeapon->get_hold_max_time();
  input_command_system->set_max_hold_time(m_HoldMaxTime);

  if(m_AmmoComp.is_ammo_empty(m_CurrentWeapon))
  {
    EventBus::get_singleton()->emit_signal("ammo_finished");
  }
}

void WeaponManager::_physics_update(double delta)
{
  // Get space state in _process or some other function could cause errors because
  // for some reason it's not accessible unless it's in the _physics_update

  Vector3 ray_start = m_WeaponMuzzleMarker->get_global_position();
  Vector3 forwardVector = m_WeaponMuzzleMarker->get_global_basis().get_column(2);
  Vector3 ray_end = ray_start + forwardVector * 1000.0f;

  // DebugDraw3D::draw_line(ray_start, ray_end, Color(1, 1, 0));

  m_SpaceState = m_CharacterBody->get_world_3d()->get_direct_space_state();
  m_Query = PhysicsRayQueryParameters3D::create(ray_start, ray_end);

  ERR_FAIL_COND_MSG(!m_Query.is_valid(), "Query is not valid!");
  ERR_FAIL_COND_MSG(!m_SpaceState, "Space State is not valid!");

  m_Query->set_collide_with_bodies(true);
  m_Result = m_SpaceState->intersect_ray(m_Query);
}

// Runs when you switch a weapon (Only once)
void WeaponManager::_update_weapon_data(Ref<Weapon> nextWeapon)
{
  m_WeaponWrapperInst = m_WeaponNodes[m_WeaponIndex]->get_node<WeaponWrapper>(NodePath("WeaponWrapper"));
  m_MuzzleComp = m_WeaponWrapperInst->get_muzzle_flash_component();
  m_WeaponMuzzleMarker = m_WeaponWrapperInst->get_muzzle_point_marker();
  m_Skeleton3D = m_WeaponWrapperInst->get_armature_skeleton();

  if(animations_from_weapon)
    play_anim_component = m_WeaponWrapperInst->get_play_anim_component();

  m_DecalScene = nextWeapon->get_weaponDecalResource();
}

void WeaponManager::_apply_damage(Node* hitBody)
{
  // FIXME: Not so nice way to get the health component but FIX later.
  if(hitBody->has_node("HealthComponent"))
  {
    HealthComponent* health_comp = hitBody->get_node<HealthComponent>("HealthComponent");
    health_comp->_take_damage(20, hitBody);
  }
}

void WeaponManager::_generate_decal()
{
  CollisionObject3D* colliderBody = nullptr;
  Node* hitBody = nullptr;

  // for(int i = 0; i < m_CurrentWeapon->get_noOfProjectilesAtSameTime(); i++)
  {

    if(!m_Result.is_empty())
    {
      m_BulletDecalInstNode = m_DecalScene->instantiate();
      m_BulletDecalNode = Object::cast_to<Decal>(m_BulletDecalInstNode);

      if(m_Result["collider"])
      {
        colliderBody = Object::cast_to<CollisionObject3D>(m_Result["collider"]);
        hitBody = Object::cast_to<Node>(m_Result["collider"]);
        colliderBody->add_child(m_BulletDecalNode);
      }  
      
      Vector3 position = Vector3(m_Result["position"]);
      m_BulletDecalNode->set_global_position(position);
      m_BulletDecalNode->look_at(m_BulletDecalNode->get_global_transform().origin + m_Result["normal"], Vector3(0.0f, 1.0f, 0.0f));
      m_BulletDecalNode->rotate_object_local(Vector3(1.0f, 0.0f, 0.0f), 90.0f);

      _apply_damage(hitBody); 
    }
  }
}

void WeaponManager::_on_weapon_anim_started(const StringName& anim_name)
{

  if(anim_name == play_anim_component->get_anim_name(AnimTypes::WEAPON_SHOOT) && play_anim_component->has_valid_anim(AnimTypes::WEAPON_SHOOT))
  {
    m_MuzzleComp->_set_particles_status(true);
    m_MuzzleLightTimeout = m_MuzzleComp->get_particle_lifetime();

    m_AmmoComp.consume_ammo(m_CurrentWeapon, 1);

    _generate_decal();
    EventBus::get_singleton()->emit_signal("weapon_fired", m_CurrentWeapon, get_owner());
  }

  if(anim_name == play_anim_component->get_anim_name(AnimTypes::WEAPON_EQUIP) && play_anim_component->has_valid_anim(AnimTypes::WEAPON_EQUIP))
  {
    m_WeaponStateCtx.IsEquipOver = false;
  }

  if(anim_name == play_anim_component->get_anim_name(AnimTypes::WEAPON_RELOAD) && play_anim_component->has_valid_anim(AnimTypes::WEAPON_RELOAD))
  {
    EventBus::get_singleton()->emit_signal("weapon_reload_start", m_Skeleton3D, get_owner());
  }
}

void WeaponManager::_on_weapon_anim_finished(const StringName& anim_name)
{
  // Make sure the reload state is over for any type of reload once the animation ends
  if(anim_name == play_anim_component->get_anim_name(AnimTypes::WEAPON_RELOAD) && play_anim_component->has_valid_anim(AnimTypes::WEAPON_RELOAD))
  {
    m_WeaponStateCtx.IsReloading = false;
  }

  if(anim_name == play_anim_component->get_anim_name(AnimTypes::WEAPON_EQUIP) && play_anim_component->has_valid_anim(AnimTypes::WEAPON_EQUIP))
  {
    m_WeaponStateCtx.IsEquipOver = true;
  }

  if(anim_name == play_anim_component->get_anim_name(AnimTypes::WEAPON_SHOOT) && play_anim_component->has_valid_anim(AnimTypes::WEAPON_SHOOT))
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
    if(anim_name == play_anim_component->get_anim_name(AnimTypes::WEAPON_RELOAD_START))
    {
      play_anim_component->execute_anim(AnimTypes::WEAPON_RELOAD);
    }

    if(anim_name == play_anim_component->get_anim_name(AnimTypes::WEAPON_RELOAD))
    {
      m_AmmoComp.set_current_weapon_ammo(m_CurrentWeapon, current_ammo + 1);
      m_AmmoComp.set_current_weapon_reserve_ammo(m_CurrentWeapon, current_reserve_ammo - 1);
      
      current_ammo = m_AmmoComp.get_current_weapon_ammo(m_CurrentWeapon); // ammo that's currently in the magazine
      current_reserve_ammo = m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon); // reserve ammo
      ammoNeeded = max_mag_capacity - current_ammo;
      ammoToBeReloaded = Math::min(ammoNeeded, current_reserve_ammo);


      if(ammoToBeReloaded == 0)
      {
        play_anim_component->execute_anim(AnimTypes::WEAPON_RELOAD_END);

        m_WeaponStateCtx.IsReloading = false;
        m_WeaponStateCtx.IsReloadStarted = false;
      }
      
      if(ammoToBeReloaded > 0)
      {
        play_anim_component->execute_anim(AnimTypes::WEAPON_RELOAD);
      }
      
    }

  }
  

}

///////////////////////////////////////////////////////////////////////
/////////////////// Weapon State Implementation ///////////////////////
///////////////////////////////////////////////////////////////////////
void WeaponManager::_equip_weapon()
{
  play_anim_component->execute_anim(AnimTypes::WEAPON_EQUIP);
}

void WeaponManager::_unequip_weapon()
{
  // Make sure to set IsReloading to false so that if the player switches a weapon mid reload the transforms don't mess and make IsReloading to true indefinitely
  m_WeaponStateCtx.IsReloading = false;

  // This function takes the weapon index from the input system and then inside assigns to the actual m_WeaponIdx used by the manager
  _switch_weapon_data(input_command_system->get_weapon_idx());

  if(weapon_component->get_next_weapon_name() != m_CurrentWeapon->get_weaponName())
  {
    // if(m_CurrentWeaponAnimPlayer->get_current_animation() != play_anim_component->get_anim_name(AnimTypes::WEAPON_UNEQUIP))
    {
      m_WeaponStateCtx.CanUnequip = true;

      // The unequip anim for the current weapon is played here.
      play_anim_component->execute_anim(AnimTypes::WEAPON_UNEQUIP);
    } 
  } else {
    m_WeaponStateCtx.CanUnequip = false;
  }
}

void WeaponManager::_shoot_weapon(double delta)
{
  // Don't even shoot, just switch to the idle state instead
  if(m_AmmoComp.is_ammo_empty(m_CurrentWeapon) ||
    (m_AmmoComp.is_ammo_empty(m_CurrentWeapon) && m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon) == 0))
  {
    print_line(get_owner()->get_name(), ": ", "Ammo empty!");
    m_MuzzleComp->_enable_light_status(false);
    m_WeaponStateCtx.ShootTimeBeforeIdle = 0.0f;
    return;
  }

  WeaponCommand& weapon_cmd = input_command_system->get_weapon_command();
  m_WeaponStateCtx.IsShooting = true;

  // Start the timer (which gives a grace period before switching to idle state of the weapon) if it's less than or equal to 0.0f
  if(m_WeaponStateCtx.ShootTimeBeforeIdle >= 0.0f)
  {
    m_WeaponStateCtx.ShootTimeBeforeIdle -= delta;
  }

  if(m_TimeBetweenShots >= 0.0f)
    m_TimeBetweenShots -= delta;
 
  /*
    Two states (for now), one for automatic handling (mouse hold) and the other for just manual handling (single click). 

    ShootTimeBeforeIdle should be self-explainatory and it resets every time you shoot the weapon to 1.0f.
    This function triggers if the weapon's time_between_shots (again should be self-explainatory) is 0.0f which is checked in weapon_states.

    The state goes to idle if ShootTimeBeforeIdle is 0.0f only. Checking for inputs could break semi-auto, full-auto weapons, so a timer is more solid.
    The Muzzle flash is handled by using a timeout variable which is set to every weapon's particle's lifetime. It's managed using a simple timer.
  */
  if(m_TimeBetweenShots <= 0.0f)
  {
    // Check whether the fire key is held or not (for automatic weapons)
    if(weapon_cmd.WantsToHoldTrigger &&
      (m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::AUTO || m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::BOTH)) 
    {
      play_anim_component->execute_anim(AnimTypes::WEAPON_SHOOT);
    }  
    
    // Check whether we pressed the fire key (manual), we don't check for wants_to_shoot_weapon() because it's one frame-state and not a persistent state
    if(weapon_cmd.WantsToPressTrigger && 
      (m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::MANUAL || m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::BOTH))
    {
      StringName execute_anim = play_anim_component->get_anim_name(AnimTypes::WEAPON_SHOOT);
  
      play_anim_component->execute_anim(AnimTypes::WEAPON_SHOOT);
    }
    
    if(weapon_cmd.WantsToHoldTrigger || weapon_cmd.WantsToPressTrigger)
    {
      m_MuzzleComp->_enable_light_status(true);
      
      m_TimeBetweenShots = m_CurrentWeapon->get_time_between_shots();
      m_WeaponStateCtx.ShootTimeBeforeIdle = MAX_SHOOT_STATE_TIME;
      weapon_cmd.WantsToPressTrigger = false;
      weapon_cmd.WantsToHoldTrigger = false;
    }
  }
      
  {
    m_MuzzleLightTimeout -= delta;
    if(m_MuzzleLightTimeout >= 0.0f)
    {
      m_MuzzleComp->_enable_light_status(true);
    }
    
    if(m_MuzzleLightTimeout <= 0.0f)
      m_MuzzleComp->_enable_light_status(false);
  }
}

void WeaponManager::_shoot_weapon_over()
{
  m_WeaponStateCtx.IsShooting = false;
  m_HoldCounter = 0.0f;
  
  m_MuzzleComp->_enable_light_status(false);
}
  
void WeaponManager::_reload_weapon()
{
  int current_ammo = m_AmmoComp.get_current_weapon_ammo(m_CurrentWeapon); // ammo that's currently in the magazine
  int current_reserve_ammo = m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon); // reserve ammo
  int max_mag_capacity = m_CurrentWeapon->get_magAmmoCount(); // total capacity of the magazine (read only)
  
  WeaponCommand& weapon_cmd = input_command_system->get_weapon_command();

  if(current_ammo >= max_mag_capacity || current_reserve_ammo <= 0)
    return;

  int ammoNeeded = max_mag_capacity - current_ammo;
  int ammoToBeReloaded = Math::min(ammoNeeded, current_reserve_ammo);
  
  m_WeaponStateCtx.IsReloading = true;
  
  if(m_CurrentWeapon->get_is_incremental_reload())
  {
    // If we shoot mid-reload just cancel the entire reload
    if(m_WeaponStateCtx.IsReloadStarted == false)
    {
      m_WeaponStateCtx.IsReloadStarted = true;
      play_anim_component->execute_anim(AnimTypes::WEAPON_RELOAD);
    }

  } else {
    play_anim_component->execute_anim(AnimTypes::WEAPON_RELOAD);

    // This will work for weapons that are not using incremental reloads
    // The ammo won't be set if you change the weapon before the mag is entered
    m_AmmoComp.set_current_weapon_ammo(m_CurrentWeapon, current_ammo + ammoToBeReloaded);
    m_AmmoComp.set_current_weapon_reserve_ammo(m_CurrentWeapon, current_reserve_ammo - ammoToBeReloaded);
  }
}

void WeaponManager::_weapon_unequip_over()
{
  weapon_component->set_current_weapon_res(weapon_component->get_next_weapon());
  EventBus::get_singleton()->emit_signal("weapon_switched", weapon_component->get_current_weapon_res(), get_owner());
}


void WeaponManager::_weapon_switch()
{
  Array weapon_list = weapon_component->get_weapon_res_list();
  m_WeaponIndex = -1;
  for (int i = 0; i < weapon_list.size(); i++) {
    Ref<Weapon> weaponRes = weapon_list[i]; 

    if(!weaponRes.is_valid())
      continue;

    if (weaponRes->get_weaponName() == weapon_component->get_next_weapon_name()) {
      m_WeaponIndex = i;
      break;
    }
  }
  
  // If the weapon is valid
  if(m_WeaponIndex != -1)
  {
    Ref<Weapon> nextWeapon = weapon_list[m_WeaponIndex];
    weapon_component->set_current_weapon_res(nextWeapon);
    _update_weapon_data(nextWeapon);
    m_WeaponStateCtx.IsWeaponSwitched = true;
  } else {
    print_error("Can't switch weapon, weapon not found!");
  }
}


void WeaponManager::_switch_weapon_data(int weaponIndex)
{
  // We are still in the previous weapon and didn't switch to the next one yet
  Ref<Weapon> next_weapon = weapon_component->get_weapon_res_list()[weaponIndex];
  
  weapon_component->set_next_weapon(next_weapon);
  weapon_component->set_next_weapon_name(next_weapon->get_weaponName());
  m_WeaponIndex = weaponIndex;
}