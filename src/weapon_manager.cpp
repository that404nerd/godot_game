#include "weapon_manager.h"
#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/transform3d.hpp"

WeaponManager::WeaponManager()
{

}

void WeaponManager::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_weapon_list", "weaponList"), &WeaponManager::set_weapon_list);
    ClassDB::bind_method(D_METHOD("get_weapon_list"), &WeaponManager::get_weapon_list);

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "weapon_list"), "set_weapon_list", "get_weapon_list");
}

Array WeaponManager::get_weapon_list() { return m_WeaponList; }
void WeaponManager::set_weapon_list(const Array& weaponList) { m_WeaponList = weaponList; }

void WeaponManager::_ready()
{
    // m_WeaponAnimPlayer = get_node<AnimationPlayer>(NodePath("WeaponAnimPlayer"));
    m_WeaponSocket = get_node<Node3D>(NodePath("../CameraController/PlayerHead/Camera3D/WeaponSocket"));
    init();
}

void WeaponManager::init()
{
    m_CurrentWeapon = m_WeaponList[0];

    // The weapon socket is already positioned manually in the editor.
    Node3D* weapon_node = Object::cast_to<Node3D>(m_CurrentWeapon->get_weapon_model()->instantiate());
    m_WeaponSocket->add_child(weapon_node);

    // if (m_CurrentWeapon.is_valid())
    //     // m_WeaponAnimPlayer->queue(m_CurrentWeapon->get_equip_anim_name()); 
    // else
    //     print_error("Current Weapon is null");
}

WeaponManager::~WeaponManager()
{

}