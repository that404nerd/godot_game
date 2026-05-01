#include "ammo_component.h"

void AmmoComponent::consume_ammo(Ref<Weapon> currentWeapon, int ammoCount)
{
  m_TotalAmmo -= ammoCount;
  m_CurrentAmmo = m_TotalAmmo;
}

void AmmoComponent::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("get_current_weapon_ammo"), &AmmoComponent::get_current_weapon_ammo);
}