#include "ammo_component.h"

void AmmoComponent::_init_data(Array weaponList)
{
  for(auto weapon : weaponList)
  {
    Ref<Weapon> currentWeapon = weapon;
    m_WeaponAmmoMap[weapon].TotalAmmo = currentWeapon->get_totalAmmoCount(); 
  }
}

void AmmoComponent::consume_ammo(Ref<Weapon> currentWeapon, int ammoCount)
{
  m_WeaponAmmoMap[currentWeapon].TotalAmmo -= ammoCount;
  print_line(m_WeaponAmmoMap[currentWeapon].TotalAmmo);
}