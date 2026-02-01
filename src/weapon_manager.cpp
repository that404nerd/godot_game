#include "weapon_manager.h"

WeaponManager::WeaponManager()
{

}

void WeaponManager::_bind_methods()
{
    GD_BIND_PROPERTY(WeaponManager, weaponList, Variant::ARRAY);
}

void WeaponManager::_ready()
{
    m_WeaponHoldPoint = get_node<Node3D>(NodePath("../CameraController/PlayerHead/Camera3D/WeaponViewportContainer/WeaponViewport/WeaponCamera/HoldPoint"));
    _init_weapon();
}

void WeaponManager::_input(const Ref<InputEvent>& event)
{
}

void WeaponManager::_init_weapon()
{
    m_CurrentWeapon = weaponList[0];

    m_WeaponNode = Object::cast_to<Node3D>(m_CurrentWeapon->get_weaponScene()->instantiate());
    m_WeaponHoldPoint->add_child(m_WeaponNode);
    

  //   if(m_WeaponAnimPlayer) {
  //       m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName());
  //   } else {
  //     print_error("Can't play equip animation!!");
  //   }
}

void WeaponManager::_physics_process(double delta)
{
}

WeaponManager::~WeaponManager()
{
}
