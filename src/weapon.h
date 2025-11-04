#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

using namespace godot;

class Weapon : public Resource {
    GDCLASS(Weapon, Resource)

public:
    Weapon();

    int get_ammo_count();
    void set_ammo_count(int ammoCount);
    
    String get_weapon_name();
    void set_weapon_name(String weaponName);

    String get_equip_anim_name();
    void set_equip_anim_name(String equipAnimName);
    
    Ref<PackedScene> get_weapon_model();
    void set_weapon_model(const Ref<PackedScene>& weaponModel);

    ~Weapon();
protected: 
    static void _bind_methods();

private:
    String m_WeaponName;
    String m_EquipAnimName;

    int m_AmmoCount;
    Ref<PackedScene> m_WeaponModel;
};
