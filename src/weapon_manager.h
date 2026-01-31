#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "globals.h"
#include "weapon.h"
#include "player.h"

#include "player_state_machine.h"

using namespace godot;

class WeaponManager : public Node {

    GDCLASS(WeaponManager, Node)

public:
    WeaponManager();

    void _ready() override;
    void _init_weapon();

    void _input(const Ref<InputEvent>& event) override;

    void _physics_process(double delta) override;

    void _weapon_sway(double delta);

    ~WeaponManager();

protected:
    static void _bind_methods();

private:
    AnimationPlayer* m_WeaponAnimPlayer { nullptr };

    Ref<Weapon> m_CurrentWeapon { nullptr };
    Node3D* m_WeaponHoldPoint { nullptr };
    Node3D* m_WeaponNode { nullptr }; // Weapon node is the actual weapon itself positioned in the weapon socket

    Vector2 m_MouseMovement { Vector2(0.0f, 0.0f) };
    Vector3 m_Position { Vector3(0.0f, 0.0f, 0.0f) }, m_Rotation { Vector3(0.0f, 0.0f, 0.0f) }, m_Scale { Vector3(0.0f, 0.0f, 0.0f) };
    Vector2 m_SwayRandAmt { Vector2(0.0f, 0.0f) };

    float m_RandSwayX { 0.0f }, m_RandSwayY { 0.0f };
    float m_RandSwayAmt { 0.0f }, m_IdleSwayRotStr { 0.0f }, m_IdleSwayAdj { 0.0f };
    float m_Time { 0.0f }; 

    Array m_CurrentWeaponList; // This is the list that will have all the current weapons the player has equiped 

    Player* m_PlayerInst { nullptr };

private:
    GD_DEFINE_PROPERTY(float, sway_speed, 0.0f);
    GD_DEFINE_PROPERTY(Array, weaponList, Array());

};
