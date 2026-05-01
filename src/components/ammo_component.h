#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>

#include "../globals.h"
#include "../weapon.h"

using namespace godot;

class AmmoComponent : public Node {

  GDCLASS(AmmoComponent, Node);

public:

  void consume_ammo(Ref<Weapon> currentWeapon, int ammoCount);

  int get_current_weapon_ammo() { return m_CurrentAmmo; }
  void set_current_weapon_ammo(Ref<Weapon> currentWeapon)
  {
    m_TotalAmmo = currentWeapon->get_totalAmmoCount();
    m_CurrentAmmo = m_TotalAmmo;
  }

  void set_total_weapon_ammo(Ref<Weapon> currentWeapon) { 
    m_TotalAmmo = currentWeapon->get_totalAmmoCount();
    m_CurrentAmmo = m_TotalAmmo;
  }

protected:
  static void _bind_methods();

private:
  int m_TotalAmmo { 0 };
  int m_CurrentAmmo { 0 };
};
