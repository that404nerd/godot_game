#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "weapon.h"

using namespace godot;

class WeaponManager : public Node3D {

    GDCLASS(WeaponManager, Node3D)

public:
    WeaponManager();

    void _ready() override;
    void init();

    void _input(const Ref<InputEvent>& event) override;

    void _physics_process(double delta) override;

    void _weapon_sway(double delta);

    ~WeaponManager();

public:
    Array get_weapon_list();
    void set_weapon_list(const Array& weaponlist);

protected:
    static void _bind_methods();
private:
    AnimationPlayer* m_WeaponAnimPlayer;

    Ref<Weapon> m_CurrentWeapon;
    Node3D* m_WeaponSocket;
    Node3D* m_WeaponNode;

    Vector2 m_MouseMovement;
    Vector2 m_Position, m_Rotation;

    Array m_WeaponList; // This is the list that will be populated with data in the editor
    Array m_CurrentWeaponList; // This is the list that will have all the current weapons the player has equiped 
};