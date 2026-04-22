#pragma once

#include "../weapon.h"

class WeaponComponent : public Node
{
  GDCLASS(WeaponComponent, Node);

public:

  void set_current_weapon(Ref<Weapon> newWeapon) { m_CurrentWeapon = newWeapon; }
  const Ref<Weapon> get_current_weapon_data() const { return m_CurrentWeapon; }

  void set_next_weapon(Ref<Weapon> nextWeapon) { m_NextWeapon = nextWeapon; }
  const Ref<Weapon> get_next_weapon_data() const { return m_NextWeapon; }

  void set_next_weapon_name(StringName nextWeaponName) { m_NextWeaponName = nextWeaponName; }
  StringName get_next_weapon_name() { return m_NextWeaponName; }

protected:
  static void _bind_methods()
  {
    GD_BIND_PROPERTY(WeaponComponent, weapon_resource_list, Variant::ARRAY); 
  }

private:
  GD_DEFINE_PROPERTY(Array, weapon_resource_list, Array());

  Array m_WeaponNodeGroup;

  StringName m_NextWeaponName;
  Ref<Weapon> m_CurrentWeapon { nullptr };
  Ref<Weapon> m_NextWeapon { nullptr };
};
