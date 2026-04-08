#pragma once

#include "../weapon.h"

class WeaponComponent : public Node
{
  GDCLASS(WeaponComponent, Node)

public:
  void _ready() override
  {

  }
  
  void set_current_weapon(Ref<Weapon> newWeapon) { m_CurrentWeapon = newWeapon; }
  Ref<Weapon> get_current_weapon_data() { return m_CurrentWeapon; }

  void _physics_process(double delta) override
  {
  }


protected:
  static void _bind_methods()
  {

  }

private:
  Ref<Weapon> m_CurrentWeapon;
};
