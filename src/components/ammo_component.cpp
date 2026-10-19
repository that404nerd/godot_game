#include "ammo_component.h"

void AmmoComponent::_init_data(Array weaponList)
{
  for(auto weapon : weaponList)
  {
    Ref<Weapon> currentWeapon = weapon;
    m_WeaponAmmoMap[weapon].TotalAmmo = currentWeapon->get_totalAmmoCount(); 
    m_WeaponAmmoMap[weapon].CurrentAmmo = currentWeapon->get_magAmmoCount(); 
    m_WeaponAmmoMap[weapon].ReserveAmmo = currentWeapon->get_totalAmmoCount() - currentWeapon->get_magAmmoCount(); 
  }
}

void AmmoComponent::_bind_methods()
{
  ADD_SIGNAL(MethodInfo("ammo_finished"));
}

bool AmmoComponent::is_ammo_empty(Ref<Weapon> currentWeapon)
{
  if(m_WeaponAmmoMap[currentWeapon].CurrentAmmo == 0)
    return true;
  
  return false;
}

void AmmoComponent::set_current_weapon_ammo(Ref<Weapon> currentWeapon, int ammo) { 
  m_WeaponAmmoMap[currentWeapon].CurrentAmmo = ammo; // Sets the ammo in the mag
} 

void AmmoComponent::set_current_weapon_reserve_ammo(Ref<Weapon> currentWeapon, int ammo)
{
  if(m_WeaponAmmoMap[currentWeapon].ReserveAmmo > 0)
    m_WeaponAmmoMap[currentWeapon].ReserveAmmo = ammo; // Subtracts the reserve ammo by the give ammo count
}


void AmmoComponent::consume_ammo(Ref<Weapon> currentWeapon, int ammoCount)
{
  m_WeaponAmmoMap[currentWeapon].CurrentAmmo -= ammoCount;
  m_WeaponAmmoMap[currentWeapon].BulletsConsumed += ammoCount;
}
