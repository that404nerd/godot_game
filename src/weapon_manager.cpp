#include "weapon_manager.h"

WeaponManager::WeaponManager()
{
  m_WeaponIndex = 0;
  m_MouseInput = Vector2(0.0f, 0.0f);
}

void WeaponManager::_bind_methods()
{
  GD_BIND_PROPERTY(WeaponManager, weaponList, Variant::ARRAY);

  ClassDB::bind_method(D_METHOD("_on_animation_finished", "anim_name"), &WeaponManager::_on_animation_finished);
}

void WeaponManager::_ready()
{
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
  m_HoldPointNode = get_node<Node3D>(NodePath("%WeaponHoldPoint"));

  m_WeaponAnimPlayer = get_node<AnimationPlayer>(NodePath("WeaponAnimPlayer"));
  m_WeaponAnimPlayer->connect("animation_finished", Callable(this, "_on_animation_finished"));

  m_LoadScene = ResourceLoader::get_singleton()->load("res://assets/decals/bullet_decal.tscn");
  _init_weapon();
}

void WeaponManager::_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouse_event = event;
  if(event->is_class("InputEventMouseMotion")) {
    m_MouseInput.x += -mouse_event->get_screen_relative().x * m_PlayerInst->get_mouse_sensitivity();
    m_MouseInput.y += -mouse_event->get_screen_relative().y * m_PlayerInst->get_mouse_sensitivity();
  
    _weapon_sway(m_MouseInput);
  }


  for(int i = 0; i < weaponList.size(); i++)
  {
    String inputAction = "weapon_" + String::num(i + 1, 0); // Need to match the set input action in the engine
    if(Input::get_singleton()->is_action_just_pressed(inputAction))
    {
      m_WeaponIndex = i;
      m_TempWeapon = weaponList[m_WeaponIndex];
      m_NextWeaponName = m_TempWeapon->get_weaponName();

      _unequip_weapon(m_NextWeaponName);
    }
  }

}

void WeaponManager::_init_weapon()
{
  m_CurrentWeapon = weaponList[m_WeaponIndex];
  _equip_weapon();
}

void WeaponManager::_weapon_bob(double delta)
{
  bool onFloor = m_PlayerInst->is_on_floor(); // so that bobbing doesn't occur during airborne states

  float velocity = m_PlayerInst->get_velocity().length();
  m_WeaponBobTime += delta * velocity * onFloor;

  float x_bob = Math::cos(m_WeaponBobTime * m_WeaponBobFreq * 0.5f) * m_WeaponBobAmp;
  float y_bob = Math::sin(m_WeaponBobTime * m_WeaponBobFreq) * m_WeaponBobAmp;

  Vector3 currentPos = m_PlayerInst->get_rig_hold_point()->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, m_WeaponBobSmoothVal, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, m_WeaponBobSmoothVal, (float)delta), 
    0.0f
  );

  m_PlayerInst->get_rig_hold_point()->set_position(newPos);
}

void WeaponManager::_idle_weapon_sway(double delta)
{
  m_IdleWeaponBobTime += delta * 0.5f;

  float x_bob = Math::cos(m_IdleWeaponBobTime * m_IdleWeaponBobFreq * 0.5f) * m_IdleWeaponBobAmp;
  float y_bob = Math::sin(m_IdleWeaponBobTime * m_IdleWeaponBobFreq) * m_IdleWeaponBobAmp;

  Vector3 currentPos = m_PlayerInst->get_rig_hold_point()->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, m_IdleWeaponBobSmoothVal, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, m_IdleWeaponBobSmoothVal, (float)delta), 
    0.0f
  );

  m_PlayerInst->get_rig_hold_point()->set_position(newPos);

}

void WeaponManager::_reset_weapon_sway(double delta)
{
  Vector3 m_HoldPointPos = m_HoldPointNode->get_position();
  m_HoldPointPos.x = Math::lerp(m_HoldPointNode->get_position().x, 0.0f, m_WeaponSwayResetValue * (float)delta);
  m_HoldPointPos.y = Math::lerp(m_HoldPointNode->get_position().y, 0.0f, m_WeaponSwayResetValue * (float)delta);
  m_HoldPointNode->set_position(m_HoldPointPos);
}

void WeaponManager::_weapon_sway(Vector2 sway_vector)
{
  Vector3 m_HoldPointPos = m_HoldPointNode->get_position();
  m_HoldPointPos.x -= sway_vector.x * m_WeaponSwayMult * m_PlayerInst->get_physics_process_delta_time();
  m_HoldPointPos.y += sway_vector.y * m_WeaponSwayMult * m_PlayerInst->get_physics_process_delta_time();

  m_HoldPointNode->set_position(m_HoldPointPos);
}

void WeaponManager::_equip_weapon()
{
  m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName());
}

void WeaponManager::_generate_decal()
{
  m_SpaceState = m_PlayerInst->get_player_camera()->get_world_3d()->get_direct_space_state();
  m_ScreenCenter = m_PlayerInst->get_player_camera()->get_viewport()->get_visible_rect().get_size() / 2.0f;
  Vector3 ray_start = m_PlayerInst->get_player_camera()->project_ray_origin(m_ScreenCenter);
  Vector3 ray_end = ray_start + m_PlayerInst->get_player_camera()->project_ray_normal(m_ScreenCenter) * m_GunRange;

  Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(ray_start, ray_end);
  query->set_collide_with_bodies(true);
  Dictionary result = m_SpaceState->intersect_ray(query);

  if(!result.is_empty())
  {
    Node* instance = m_LoadScene->instantiate();
    Decal* bulletDecal = Object::cast_to<Decal>(instance);
    get_tree()->get_root()->add_child(bulletDecal);
    bulletDecal->set_global_position(result["position"]);
    bulletDecal->look_at(bulletDecal->get_global_transform().origin + result["normal"], Vector3(0.0f, 1.0f, 0.0f));
    bulletDecal->rotate_object_local(Vector3(1.0f, 0.0f, 0.0f), 90.0f);
   }

}

void WeaponManager::_shoot()
{
  if(m_WeaponAnimPlayer)
  {
    m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName());
  }

  _generate_decal();
}

void WeaponManager::_unequip_weapon(const StringName& nextWeaponName)
{
  if(nextWeaponName != m_CurrentWeapon->get_weaponName())
  {
    if(m_WeaponAnimPlayer->get_current_animation() != m_CurrentWeapon->get_weaponUnequipAnimName())
    {
      m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponUnequipAnimName()); // Plays the unequip animation of the current weapon
      m_NextWeaponName = nextWeaponName;
    }
  }

}

void WeaponManager::_change_weapon(const StringName& weaponName)
{
  int weapon_index = -1;

  for (int i = 0; i < weaponList.size(); i++) {
    Ref<Weapon> res = weaponList[i]; 
    
    if (res.is_valid() && res->get_weaponName() == weaponName) {
      weapon_index = i;
      break;
    }
  }

  if(weapon_index != -1)
  {
    m_CurrentWeapon = weaponList[weapon_index];
    m_NextWeaponName = "";
    _equip_weapon();
  }
}

void WeaponManager::_on_animation_finished(const StringName& anim_name)
{
  if(anim_name == m_CurrentWeapon->get_weaponUnequipAnimName())
  {
    _change_weapon(m_NextWeaponName);
  }
}

void WeaponManager::_physics_process(double delta)
{
  m_IdleWeaponBobAmp = m_CurrentWeapon->get_idle_weapon_bob_amp();
  m_IdleWeaponBobFreq = m_CurrentWeapon->get_idle_weapon_bob_freq();
  m_WeaponBobAmp = m_CurrentWeapon->get_weapon_bob_amp();
  m_WeaponBobFreq = m_CurrentWeapon->get_weapon_bob_freq();

  m_WeaponSwayMult = m_CurrentWeapon->get_weapon_sway_mult();
  m_WeaponSwayResetValue = m_CurrentWeapon->get_weapon_sway_reset();
  m_IdleWeaponBobSmoothVal = m_CurrentWeapon->get_idle_weapon_bob_smooth_val();
  m_WeaponBobSmoothVal = m_CurrentWeapon->get_weapon_bob_smooth_val();

  m_GunRange = m_CurrentWeapon->get_gun_range();

  _reset_weapon_sway(delta);

  // TODO: THIS IS PURE ASS. Need a better way to manage states
  if(m_StateMachineInst->get_current_state() == m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::SPRINT)
    || m_StateMachineInst->get_current_state() == m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::CROUCH))
    _weapon_bob(delta);

  if(m_StateMachineInst->get_current_state() == m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::IDLE))
  {
    _idle_weapon_sway(delta);
  }

  if(Input::get_singleton()->is_action_just_pressed("fire"))
  {
    _shoot();
  }

 }

WeaponManager::~WeaponManager()
{
}
