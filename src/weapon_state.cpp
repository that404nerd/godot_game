#include "weapon_state.h"

void WeaponState::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_weapon_idle", "delta"), &WeaponState::_weapon_idle);
  ClassDB::bind_method(D_METHOD("_weapon_shoot", "delta"), &WeaponState::_weapon_shoot);
  ClassDB::bind_method(D_METHOD("_weapon_reload", "delta"), &WeaponState::_weapon_reload);
}


void WeaponState::_ready()
{

}

void WeaponState::_weapon_idle(double delta)
{

}

void WeaponState::_weapon_shoot(double delta)
{

}

void WeaponState::_weapon_reload(double delta)
{

}

void WeaponState::_physics_process(double delta)
{

}
