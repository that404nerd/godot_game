#include "weapon_manager.h"

WeaponManager::WeaponManager()
{
  GameManager::get_singleton()->set_weapon_manager_inst(this);
  m_WeaponIndex = 0;
}

void WeaponManager::_bind_methods()
{
  GD_BIND_PROPERTY(WeaponManager, weaponList, Variant::ARRAY);

  ADD_SIGNAL(MethodInfo("weapon_equip"));
  ADD_SIGNAL(MethodInfo("weapon_unequip", PropertyInfo(Variant::STRING, "nextWeaponName")));
  ADD_SIGNAL(MethodInfo("weapon_shoot"));
  ADD_SIGNAL(MethodInfo("weapon_reload"));
  ADD_SIGNAL(MethodInfo("weapon_change", PropertyInfo(Variant::STRING, "nextWeaponName")));
}

void WeaponManager::_ready()
{
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
  m_HoldPointNode = get_node<Node3D>(NodePath("%WeaponHoldPoint"));

  m_CurrentWeapon = weaponList[m_WeaponIndex];
}

void WeaponManager::_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouse_event = event;
  if(event->is_class("InputEventMouseMotion")) {
    m_MouseInput.x += -mouse_event->get_screen_relative().x * m_PlayerInst->get_mouse_sensitivity();
    m_MouseInput.y += -mouse_event->get_screen_relative().y * m_PlayerInst->get_mouse_sensitivity();
  
    _weapon_sway(m_MouseInput);
  }

  if(Input::get_singleton()->is_action_just_pressed("reload_weapon"))
    emit_signal("weapon_reload");
}

void WeaponManager::_weapon_bob(double delta)
{
  bool onFloor = m_PlayerInst->is_on_floor(); // so that bobbing doesn't occur during airborne states

  float velocity = m_PlayerInst->get_velocity().length();
  m_WeaponBobTime += delta * velocity * onFloor;

  float x_bob = Math::cos(m_WeaponBobTime * m_WeaponBobFreq * 0.5f) * m_WeaponBobAmp;
  float y_bob = Math::sin(m_WeaponBobTime * m_WeaponBobFreq) * m_WeaponBobAmp;

  Vector3 currentPos = m_HoldPointNode->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, m_WeaponBobSmoothVal, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, m_WeaponBobSmoothVal, (float)delta), 
    0.0f
  );
  
  if(!m_HoldPointNode->get_transform().is_finite())
  {
    print_error("Weapon BOB: Transform is infinite!");
    GENERATE_TRAP();
  }
  m_HoldPointNode->set_position(newPos);
}

void WeaponManager::_idle_weapon_sway(double delta)
{
  m_IdleWeaponBobTime += delta * 0.5f;

  float x_bob = Math::cos(m_IdleWeaponBobTime * m_IdleWeaponBobFreq * 0.5f) * m_IdleWeaponBobAmp;
  float y_bob = Math::sin(m_IdleWeaponBobTime * m_IdleWeaponBobFreq) * m_IdleWeaponBobAmp;

  Vector3 currentPos = m_HoldPointNode->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, m_IdleWeaponBobSmoothVal, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, m_IdleWeaponBobSmoothVal, (float)delta), 
    0.0f
  );

  if(!m_HoldPointNode->get_transform().is_finite())
  {
    print_error("Idle weapon sway: Transform is infinite!");
    GENERATE_TRAP();
  }
  m_HoldPointNode->set_position(newPos);

}

void WeaponManager::_reset_weapon_sway(double delta)
{
  Vector3 m_HoldPointPos = m_HoldPointNode->get_position();
  m_HoldPointPos.x = Math::lerp(m_HoldPointNode->get_position().x, 0.0f, m_WeaponSwayResetValue * (float)delta);
  m_HoldPointPos.y = Math::lerp(m_HoldPointNode->get_position().y, 0.0f, m_WeaponSwayResetValue * (float)delta);
  if(!m_HoldPointNode->get_transform().is_finite())
  {
    print_error("Reset Weapon Sway: Transform is infinite!");
    GENERATE_TRAP();
  }
  m_HoldPointNode->set_position(m_HoldPointPos);
}

void WeaponManager::_weapon_sway(Vector2 sway_vector)
{
  Vector3 m_HoldPointPos = m_HoldPointNode->get_position();
  m_HoldPointPos.x -= sway_vector.x * m_WeaponSwayMult * m_PlayerInst->get_physics_process_delta_time();
  m_HoldPointPos.y += sway_vector.y * m_WeaponSwayMult * m_PlayerInst->get_physics_process_delta_time();

  if(!m_HoldPointNode->get_transform().is_finite())
  {
    print_error("Weapon Sway: Transform is infinite!");
    GENERATE_TRAP();
  }
  m_HoldPointNode->set_position(m_HoldPointPos);
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
  m_CurrentStateName = m_StateMachineInst->get_current_state();
  
  
  if(m_CurrentStateName == StringName("Sprint") || m_CurrentStateName == StringName("Crouch"))
    _weapon_bob(delta);

  // <= 0.1f because I observed sometimes it's not perfectly 0.0f. This should fix it
  if(m_PlayerInst->get_velocity().length() <= 0.1f)
  {
    _idle_weapon_sway(delta);
  }

  if(Input::get_singleton()->is_action_just_pressed("fire"))
  {
    emit_signal("weapon_shoot");
  }

  _reset_weapon_sway(delta);
  m_MouseInput = Vector2(0.0f, 0.0f);
}

WeaponManager::~WeaponManager()
{
}
