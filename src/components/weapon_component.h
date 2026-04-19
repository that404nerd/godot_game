#pragma once

#include "../weapon.h"

class WeaponComponent : public Node
{
  GDCLASS(WeaponComponent, Node);

public:
  void _ready() override
  {
    if(!weapon_resource_list.is_empty())
      m_CurrentWeapon = weapon_resource_list[0];
  }

  void set_current_weapon(Ref<Weapon> newWeapon) { m_CurrentWeapon = newWeapon; }
  const Ref<Weapon> get_current_weapon_data() const { return m_CurrentWeapon; }

protected:
  static void _bind_methods()
  {
    GD_BIND_PROPERTY(WeaponComponent, weapon_resource_list, Variant::ARRAY); 
  }

private:
  GD_DEFINE_PROPERTY(Array, weapon_resource_list, Array());
  Ref<Weapon> m_CurrentWeapon { nullptr };
};
