#include "weapon.h"

using namespace godot;

Weapon::Weapon() :  m_WeaponName(""), m_EquipAnimName(""), m_AmmoCount(0), m_WeaponModel(nullptr) {}

void Weapon::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_ammo_count", "ammoCount"), &Weapon::set_ammo_count);
    ClassDB::bind_method(D_METHOD("get_ammo_count"), &Weapon::get_ammo_count);
    
    ClassDB::bind_method(D_METHOD("set_weapon_name", "weaponName"), &Weapon::set_weapon_name);
    ClassDB::bind_method(D_METHOD("get_weapon_name"), &Weapon::get_weapon_name);

    ClassDB::bind_method(D_METHOD("set_equip_anim_name", "equipAnimName"), &Weapon::set_equip_anim_name);
    ClassDB::bind_method(D_METHOD("get_equip_anim_name"), &Weapon::get_equip_anim_name);
    
    ClassDB::bind_method(D_METHOD("set_weapon_model", "weaponModel"), &Weapon::set_weapon_model);
    ClassDB::bind_method(D_METHOD("get_weapon_model"), &Weapon::get_weapon_model);
    
    ADD_PROPERTY(PropertyInfo(Variant::INT, "ammo"), "set_ammo_count", "get_ammo_count");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "weapon_name"), "set_weapon_name", "get_weapon_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "weapon_equip_anim_name"), "set_equip_anim_name", "get_equip_anim_name");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "weapon_model", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_weapon_model", "get_weapon_model");
}

// Setters/Getters

int Weapon::get_ammo_count() { return m_AmmoCount; }
void Weapon::set_ammo_count(const int ammoCount) { m_AmmoCount = ammoCount; }

String Weapon::get_weapon_name() { return m_WeaponName; }
void Weapon::set_weapon_name(const String weaponName) { m_WeaponName = weaponName; }

String Weapon::get_equip_anim_name() { return m_EquipAnimName; }
void Weapon::set_equip_anim_name(const String equipAnimName) { m_EquipAnimName = equipAnimName; }

Ref<PackedScene> Weapon::get_weapon_model() { return m_WeaponModel; }
void Weapon::set_weapon_model(const Ref<PackedScene>& weaponModel) { m_WeaponModel = weaponModel; }

Weapon::~Weapon() {}