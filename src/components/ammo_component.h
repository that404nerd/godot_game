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

class AmmoComponent : public Node {
  GDCLASS(AmmoComponent, Node);
public:

  void _init_data(Array weaponList);
  void consume_ammo(Ref<Weapon> currentWeapon, int ammoCount);

  void set_current_weapon_ammo(Ref<Weapon> currentWeapon, int ammo);
  void set_current_weapon_reserve_ammo(Ref<Weapon> currentWeapon, int ammo);

  bool is_ammo_empty(Ref<Weapon> currentWeapon);

  int get_current_weapon_ammo(Ref<Weapon> currentWeapon) { return m_WeaponAmmoMap[currentWeapon].CurrentAmmo; }
  int get_current_weapon_reserve_ammo(Ref<Weapon> currentWeapon) { return m_WeaponAmmoMap[currentWeapon].ReserveAmmo; }

protected:
  static void _bind_methods();

private:
  AHashMap<Ref<Weapon>, AmmoData> m_WeaponAmmoMap;
};
