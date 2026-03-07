#include "weapon.h"

using namespace godot;

Weapon::Weapon()
{

}

void Weapon::_bind_methods() {

  ADD_GROUP("General Weapon Properties", "");
  GD_BIND_PROPERTY(Weapon, ammoCount, Variant::INT);
  GD_BIND_PROPERTY(Weapon, weaponName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, shoot_delay, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, reload_time, Variant::FLOAT);

  ADD_GROUP("Weapon Animation Names", "");
  GD_BIND_PROPERTY(Weapon, weaponEquipAnimName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, weaponUnequipAnimName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, weaponShootingAnimName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, weaponReloadAnimName, Variant::STRING);

  ADD_GROUP("Weapon Bob And Range", "");
  GD_BIND_PROPERTY(Weapon, idle_weapon_bob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, idle_weapon_bob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_bob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_bob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, gun_range, Variant::FLOAT);

  ADD_GROUP("Weapon Bob Multipliers and Smoothing Properties", "");
  GD_BIND_PROPERTY(Weapon, weapon_sway_mult, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_sway_reset, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, idle_weapon_bob_smooth_val, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_bob_smooth_val, Variant::FLOAT);
}

Weapon::~Weapon() {}
