#include "weapon_manager.h"
#include "weapon_states.h"
#include "weapon_state_machine.h"

void WeaponManager::_ready()
{
  weapon_component->set_current_weapon(weapon_component->get_weapon_resource_list()[m_WeaponIndex]);
  m_WeaponEffects._init_data(hold_point_node, character_component, weapon_component);

  /* NOTE: This took me 2 hours to find lol, i forgot that i was dealing with different animation
  players for different weapon scene, this connects the _on_animation_finished to all animation players */

  for(int i = 0; i < hold_point_node->get_children().size(); i++)
  {
    Node3D* weapon_node = nullptr;
    AnimationPlayer* anim_player = nullptr;

    m_WeaponNodes.push_back(Object::cast_to<Node3D>(hold_point_node->get_children()[i]));
    weapon_node = Object::cast_to<Node3D>(m_WeaponNodes[i]);

    if(i != m_WeaponIndex) // Check if the first weapon index is m_WeaponIndex (0 in this case)
    {
      // Hide the other weapons except the first weapon that's gonna be equipped
      weapon_node->set_visible(false);
    }

    m_WeaponAnims.push_back(weapon_node->get_node<AnimationPlayer>("AnimationPlayer"));
    anim_player = Object::cast_to<AnimationPlayer>(m_WeaponAnims[i]);

    anim_player->connect("animation_started", Callable(this, "_on_weapon_shoot"));
    anim_player->connect("animation_finished", Callable(weapon_state_machine, "_on_animation_finished"));
  }
  
  m_CurrentWeaponAnimPlayer = m_WeaponAnims[m_WeaponIndex];
  m_MuzzleFlashNode = m_WeaponNodes[m_WeaponIndex]->get_node<Node3D>(NodePath("%MuzzleFlash"));

  m_OmniLightNode = m_MuzzleFlashNode->get_node<OmniLight3D>(NodePath("OmniLight3D"));
  m_Particles3D = m_MuzzleFlashNode->get_node<GPUParticles3D>(NodePath("GPUParticles3D"));


  // Set the current weapon right here first!
  m_CurrentWeapon = weapon_component->get_current_weapon_data();
  m_CharacterBody = character_component->get_character_body();

  m_Camera = get_node<Camera3D>(NodePath("%PlayerCamera"));
  m_ScreenCenter = get_viewport()->get_visible_rect().get_size() / 2.0f;
  m_LoadScene = ResourceLoader::get_singleton()->load("res://assets/decals/bullet_decal.tscn");

  m_AmmoComp._init_data(weapon_component->get_weapon_resource_list());
}


void WeaponManager::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_weapon_shoot", "anim_name"), &WeaponManager::_on_weapon_shoot);
  
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, hold_point_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, player_head_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

 
}

void WeaponManager::_unhandled_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouseEvent = event;
  
  if(event->is_class("InputEventMouseMotion")) {
    float swayIntensity = 0.005f; 

    // m_MouseInput.x += -mouseEvent->get_screen_relative().x * 0.003f;
    // m_MouseInput.y += -mouseEvent->get_screen_relative().y * 0.003f;

    Vector2 relative = mouseEvent->get_relative(); 
    m_MouseVel.x += -relative.x * swayIntensity;
    m_MouseVel.y += -relative.y * swayIntensity;
  }
}

void WeaponManager::_process(double delta)
{
  m_CurrentWeapon = weapon_component->get_current_weapon_data();
  m_WeaponStateCtx.CurrentWeaponType = m_CurrentWeapon->get_weapon_type();

  m_WeaponEffects._update(delta, m_MouseVel);

  // m_MouseInput.x = 0.0f;
  // m_MouseInput.y = 0.0f;
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


void WeaponManager::generate_decal()
{
  if(!m_Result.is_empty())
  {
    Node* instance = m_LoadScene->instantiate();
    Decal* bulletDecal = Object::cast_to<Decal>(instance);
    add_child(bulletDecal);
    bulletDecal->set_global_position(m_Result["position"]);
    bulletDecal->look_at(bulletDecal->get_global_transform().origin + m_Result["normal"], Vector3(0.0f, -1.0f, 0.0f));
    bulletDecal->rotate_object_local(Vector3(1.0f, 0.0f, 0.0f), 90.0f);
  }
}


///////////////////////////////////////////////////////////////////////
/////////////////// Weapon State Implementation ///////////////////////
///////////////////////////////////////////////////////////////////////
void WeaponManager::_equip_weapon()
{
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
    } else {
      m_WeaponStateCtx.CanUnequip = false;
    }
  }
}


void WeaponManager::_on_weapon_shoot(const StringName& anim_name)
{
  if(anim_name == StringName(m_CurrentWeapon->get_weaponShootingAnimName()))
  {
    m_LightTimeout = 0.05f;
    m_Particles3D->set_emitting(true);
    m_AmmoComp.consume_ammo(m_CurrentWeapon, 1);
    generate_decal();
    EventBus::get_singleton()->emit_signal("weapon_fired", m_CurrentWeapon);
  }
}

void WeaponManager::_weapon_shoot_finish()
{
}

void WeaponManager::_shoot_weapon(double delta)
{
  // Start the timer (which gives a grace period before switching to idle state of the weapon) if it's less than or equal to 0.0f
  if(m_WeaponStateCtx.ShootTimeBeforeIdle >= 0.0f)
  {
    m_WeaponStateCtx.ShootTimeBeforeIdle -= delta;
  }

  if(m_WeaponStateCtx.ShootCooldown > 0.0)
  {
    m_WeaponStateCtx.ShootCooldown -= delta;
  }
  
  // Check whether the fire key is held or not (for automatic weapons)
  if(Input::get_singleton()->is_action_pressed("shoot_weapon") && (
    m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::AUTO || m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::BOTH
  )) 
  {
    m_HoldCounter += delta;
    m_WeaponStateCtx.ShootTimeBeforeIdle = 1.0f;

    if(m_HoldCounter > m_HoldMaxTime)
    {
      m_WeaponStateCtx.IsKeyHeld = true;
    }
  } 
 
  // Check whether we pressed the fire key (manual)
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon") && (
    m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::MANUAL || m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::BOTH
  ))
  {
    m_WeaponStateCtx.IsKeyPressed = true;
    m_WeaponStateCtx.ShootTimeBeforeIdle = 1.0f;
  }
  
  if((m_WeaponStateCtx.IsKeyPressed || m_WeaponStateCtx.IsKeyHeld) && m_AmmoComp.get_current_weapon_ammo(m_CurrentWeapon) > 0)
  {
    m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName(), 
        m_CurrentWeapon->get_weapon_shoot_anim_blend(), m_CurrentWeapon->get_weapon_shoot_anim_speed());

    m_OmniLightNode->set_visible(true);
    m_WeaponStateCtx.IsKeyPressed = false;
  }

  m_LightTimeout -= delta;

  if(m_LightTimeout <= 0.0f)
    m_OmniLightNode->set_visible(false);

  
  // Set the key held to false and reset the hold counter 
  if(Input::get_singleton()->is_action_just_released("shoot_weapon")) 
  {
    m_WeaponStateCtx.IsKeyHeld = false;
    m_HoldCounter = 0.0f;
  }
}
  
void WeaponManager::_reload_weapon()
{
  int current_ammo = m_AmmoComp.get_current_weapon_ammo(m_CurrentWeapon); // ammo that's currently in the magazine
  int current_reserve_ammo = m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon); // reserve ammo
  int max_mag_capacity = m_CurrentWeapon->get_magAmmoCount(); // total capacity of the magazine (read only)

  if(current_ammo >= max_mag_capacity || current_reserve_ammo <= 0)
    return;

  m_CurrentWeaponAnimPlayer->play(
    m_CurrentWeapon->get_weaponReloadAnimName(), 
    m_CurrentWeapon->get_weapon_reload_anim_blend(), 
    m_CurrentWeapon->get_weapon_reload_anim_speed()
  );

  int ammoNeeded = max_mag_capacity - current_ammo;
  int ammoToBeReloaded = Math::min(ammoNeeded, current_reserve_ammo);

  m_AmmoComp.set_current_weapon_ammo(m_CurrentWeapon, current_ammo + ammoToBeReloaded);
  m_AmmoComp.set_current_weapon_reserve_ammo(m_CurrentWeapon, current_reserve_ammo - ammoToBeReloaded);
}

void WeaponManager::_weapon_unequip_over()
{
  m_CurrentWeaponAnimPlayer = m_WeaponAnims[m_WeaponIndex];
  weapon_component->set_current_weapon(weapon_component->get_next_weapon_data());
}

void WeaponManager::_weapon_switch()
{
  int weapon_index = -1;
  
  for (int i = 0; i < weapon_component->get_weapon_resource_list().size(); i++) {
    Ref<Weapon> res = weapon_component->get_weapon_resource_list()[i]; 

    if (res.is_valid() && res->get_weaponName() == weapon_component->get_next_weapon_name()) {
      weapon_index = i;
      break;
    }
  }
  
  if(weapon_index != -1)
  {
    Ref<Weapon> nextWeapon = weapon_component->get_weapon_resource_list()[weapon_index];
    weapon_component->set_current_weapon(nextWeapon);
    m_WeaponEffects._update_data(nextWeapon);
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

WeaponManager::~WeaponManager()
{
}