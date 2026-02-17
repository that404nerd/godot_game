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
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/capsule_mesh.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/core/math.hpp>

#include "globals.h"
#include "game_manager.h"
#include "global_state_handler.h"

#include "dd3d_cpp_api.hpp"

using namespace godot;

class Player : public CharacterBody3D {

  GDCLASS(Player, CharacterBody3D);

private:
  struct PlayerGlobalState {
    bool CanDash = true;
    float DashCooldown;
  };

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

  Node3D* get_rig_hold_point() { return m_RigHoldPoint; }
  Node3D* get_player_head() { return m_PlayerHead; }

  CollisionShape3D* get_player_standing_collider() { return m_StandingPlayerCollider; }
  CollisionShape3D* get_player_crouching_collider() { return m_CrouchingPlayerCollider; }

  Vector3 get_wish_dir() { return m_WishDir; }
  Vector2 get_input_dir() { return m_InputDir; }

  PlayerGlobalState& get_global_state() { return m_GlobalState; }

  void _update_input();
  void _update_velocity();

private:
  PlayerGlobalState m_GlobalState;

  Node3D* m_PlayerHead = nullptr;
  Node3D* m_CameraControllerNode = nullptr;
  Marker3D* m_CameraAnchor = nullptr;

  // Get Collision shapes
  CollisionShape3D* m_StandingPlayerCollider = nullptr;
  CollisionShape3D* m_CrouchingPlayerCollider = nullptr;

  Timer* m_JumpBufferTimer = nullptr;

  Camera3D* m_PlayerCamera = nullptr;

  Node3D* m_RigHoldPoint = nullptr;

private:

  // Player vectors & Input vectors
  Vector2 m_InputDir = Vector2(0.0f, 0.0f);
  Vector3 m_WishDir = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 m_PlayerTiltVector = Vector3(0.0f, 0.0f, 0.0f);

  float m_Gravity = 0.0f;

private:
  GD_DEFINE_PROPERTY(float, crouch_speed, 3.0f);
  GD_DEFINE_PROPERTY(float, sprint_speed, 10.0f);  

  GD_DEFINE_PROPERTY(float, crouch_translate, 0.8f);
  GD_DEFINE_PROPERTY(float, headbob_decay, 2.0f);
  
  GD_DEFINE_PROPERTY(float, slide_tilt_angle, 5.0f);
  GD_DEFINE_PROPERTY(float, slide_speed, 10.0f);

  GD_DEFINE_PROPERTY(float, jump_height, 10.0f);
  GD_DEFINE_PROPERTY(float, down_gravity, 15.0f);

  GD_DEFINE_PROPERTY(float, max_air_move_speed, 6.0f);
  GD_DEFINE_PROPERTY(float, max_air_accel, 7.0f);

  GD_DEFINE_PROPERTY(float, mouse_sensitivity, 0.003f);

  GD_DEFINE_PROPERTY(float, dash_cooldown, 1.0f);

  GD_DEFINE_PROPERTY(float, ground_accel, 15.0f);
  GD_DEFINE_PROPERTY(float, ground_decel, 10.0f);
  GD_DEFINE_PROPERTY(float, ground_friction, 8.0f);
};
