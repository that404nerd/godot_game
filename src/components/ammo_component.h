#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/templates/a_hash_map.hpp>

#include "../globals.h"
#include "../weapon.h"

using namespace godot;

struct AmmoData 
{
  int TotalAmmo;
  int CurrentAmmo;
  int BulletsConsumed;
};

class AmmoComponent {

public:

  void _init_data(Array weaponList);

  void consume_ammo(Ref<Weapon> currentWeapon, int ammoCount);

  void set_current_weapon_ammo(Ref<Weapon> currentWeapon, int ammo) {
    m_WeaponAmmoMap[currentWeapon].TotalAmmo = ammo;
  } 

  int get_current_weapon_ammo(Ref<Weapon> currentWeapon) {
    m_WeaponAmmoMap[currentWeapon].CurrentAmmo = m_WeaponAmmoMap[currentWeapon].TotalAmmo;
    return m_WeaponAmmoMap[currentWeapon].CurrentAmmo;
  }

private:
  AHashMap<Ref<Weapon>, AmmoData> m_WeaponAmmoMap;
};
