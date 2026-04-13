#pragma once

#include "../weapon.h"

class WeaponComponent
{
public:
  void set_current_weapon(Ref<Weapon> newWeapon) { m_CurrentWeapon = newWeapon; }
  const Ref<Weapon> get_current_weapon_data() const { return m_CurrentWeapon; }

private:
  Ref<Weapon> m_CurrentWeapon { nullptr };
};
