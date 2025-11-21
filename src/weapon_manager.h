#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include "weapon.h"

using namespace godot;

class WeaponManager : public Node3D {

    GDCLASS(WeaponManager, Node3D)

public:
    WeaponManager();

    void _ready() override;
    void init();

    ~WeaponManager();

public:
    Array get_weapon_list();
    void set_weapon_list(const Array& weaponlist);

protected:
    static void _bind_methods();
private:
    AnimationPlayer* m_WeaponAnimPlayer = nullptr;

    Ref<Weapon> m_CurrentWeapon;
    Node3D* m_WeaponSocket = nullptr;

    Array m_WeaponList; // This is the list that will be populated with data in the editor
    Array m_CurrentWeaponList; // This is the list that will have all the current weapons the player has equiped 
};