#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/marker3d.hpp>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/project_settings.hpp>

#include "globals.h"

using namespace godot;

class Player : public CharacterBody3D {

    GDCLASS(Player, CharacterBody3D);

public:
    Player();
    
    void _ready() override;
    void _physics_process(double delta) override;
    void _unhandled_input(const Ref<InputEvent>& event) override;

    static void _bind_methods();
    
    ~Player();
    
public:

    Marker3D* get_camera_anchor() { return m_CameraAnchor; }
    Camera3D* get_player_camera() { return m_PlayerCamera; }
    Node3D* get_player_head() { return m_PlayerHead; }
    Timer* get_jump_buffer_timer() { return m_JumpBufferTimer; }

    CollisionShape3D* get_player_standing_collider() { return m_StandingPlayerCollider; }
    CollisionShape3D* get_player_crouching_collider() { return m_CrouchingPlayerCollider; }
    
    Vector3 get_wish_dir() { return m_WishDir; }
    Vector2 get_input_dir() { return m_InputDir; }

    void _update_input();
    void _update_velocity();

protected:
    Node3D* m_PlayerHead = nullptr;
    Node3D* m_CameraControllerNode = nullptr;
    Marker3D* m_CameraAnchor = nullptr;
    Timer* m_JumpBufferTimer = nullptr;

    // Get Collision shapes
    CollisionShape3D* m_StandingPlayerCollider = nullptr;
    CollisionShape3D* m_CrouchingPlayerCollider = nullptr;

    Camera3D* m_PlayerCamera = nullptr;

    // Player vectors & Input vectors
    Vector2 m_InputDir = Vector2(0.0f, 0.0f);
    Vector3 m_WishDir = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 m_PlayerTiltVector = Vector3(0.0f, 0.0f, 0.0f);

    float m_Gravity = 0.0f;
};