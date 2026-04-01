#include "weapon_state.h"

void WeaponState::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_weapon_idle", "delta"), &WeaponState::_weapon_idle);
  ClassDB::bind_method(D_METHOD("_weapon_shoot", "delta"), &WeaponState::_weapon_shoot);
  ClassDB::bind_method(D_METHOD("_weapon_reload", "delta"), &WeaponState::_weapon_reload);
}

void WeaponState::_ready()
{
  m_WeaponManager = GameManager::get_singleton()->get_weapon_manager_inst();
  if(m_WeaponManager)
  {
    m_WeaponManager->connect("weapon_idle", Callable(this, "_weapon_idle"));
    m_WeaponManager->connect("weapon_shoot", Callable(this, "_weapon_shoot"));
    m_WeaponManager->connect("weapon_reload", Callable(this, "_weapon_reload"));
  } else print_error("WeaponManager not valid!");

  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
}

void WeaponState::_weapon_idle(double delta)
{
  print_line("calling from weapon idle function!");
}

void WeaponState::_weapon_shoot(double delta)
{
  print_line("calling from weapon shoot function!");
}

void WeaponState::_weapon_reload(double delta)
{
  print_line("calling from weapon reload function!");
}

void WeaponState::_physics_process(double delta)
{
  emit_signal("weapon_idle");
}
