#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/templates/a_hash_map.hpp>

#include "../globals.h"
#include "../resources/weapon.h"

using namespace godot;

struct AmmoData 
{
  int TotalAmmo;
  int CurrentAmmo;
  int BulletsConsumed;
  int ReserveAmmo;
};

class AmmoComponent {

public:

  void _init_data(Array weaponList);
  void consume_ammo(Ref<Weapon> currentWeapon, int ammoCount);

  void set_current_weapon_ammo(Ref<Weapon> currentWeapon, int ammo) { 
    m_WeaponAmmoMap[currentWeapon].CurrentAmmo = ammo; // Sets the ammo in the mag
  } 

  void set_current_weapon_reserve_ammo(Ref<Weapon> currentWeapon, int ammo)
  {
    if(m_WeaponAmmoMap[currentWeapon].ReserveAmmo > 0)
      m_WeaponAmmoMap[currentWeapon].ReserveAmmo = ammo; // Subtracts the reserve ammo by the give ammo count
  }

  bool is_ammo_empty(Ref<Weapon> currentWeapon);

  int get_current_weapon_ammo(Ref<Weapon> currentWeapon) { return m_WeaponAmmoMap[currentWeapon].CurrentAmmo; }
  int get_current_weapon_reserve_ammo(Ref<Weapon> currentWeapon) { return m_WeaponAmmoMap[currentWeapon].ReserveAmmo; }

private:
  AHashMap<Ref<Weapon>, AmmoData> m_WeaponAmmoMap;
};
