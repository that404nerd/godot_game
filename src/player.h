#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/remote_transform3d.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/marker3d.hpp>

#include "globals.h"

using namespace godot;

class Player : public CharacterBody3D {

    friend class Sprint;

    GDCLASS(Player, CharacterBody3D);

public:
    Player();

    void _ready() override;
    void _physics_process(double delta) override;

    void _handle_ground_physics(double delta);
    void _handle_air_physics(double delta);
    void _handle_crouch(double delta);
    bool _noclip(double delta);

    float get_move_speed() { return Input::get_singleton()->is_action_pressed("crouch") ? Globals::CrouchSpeed : Globals::SprintSpeed; }

    void headbob_effect(double delta);

    static void _bind_methods();

    Marker3D* GetCameraAnchor() { return m_CameraAnchor; }

    ~Player();

protected:
    Node3D* m_PlayerHead = nullptr;
    Node3D* m_CameraControllerNode = nullptr;
    Marker3D* m_CameraAnchor = nullptr;

    Timer* m_JumpBufferTimer = nullptr;

    // Get Collision shapes
    CollisionShape3D* m_StandingCollisionShape = nullptr;

    Camera3D* m_PlayerCamera = nullptr;

    // Player vectors & Input vectors
    Vector2 m_InputDir = Vector2(0.0f, 0.0f);
    Vector3 m_WishDir = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 m_CamWishDir = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 m_PlayerTiltVector = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 m_PlayerVel = Vector3(0.0f, 0.0f, 0.0f);

    Vector2 m_SlideVector;
    Vector2 m_JumpVector;

    float m_HeadBobTime = 0.0f;
    bool m_IsNoClip = false;

    int m_CurrentJumps;

    // Ground physics variables  
    float m_GroundAccel = 14.0f;
    float m_GroundDecel = 10.0f;
    float m_GroundFriction = 6.0f; 

    // Movement states
    bool m_IsFalling = false;
    bool m_IsCrouching = false;
    bool m_IsSliding = false;
    bool m_IsMoving = false;
    bool m_IsJumping = false;
    bool m_IsDashing = false;
    bool m_HasJumped = false;
};
