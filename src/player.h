#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "components/character_component.h"

#include "globals.h"
#include "global_state_handler.h"
#include "weapon_manager.h"

using namespace godot;

class movementStateMachine;

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
  void _process(double delta) override;
  void _physics_process(double delta) override;
  void _unhandled_input(const Ref<InputEvent>& event) override;

  static void _bind_methods();
  
  ~Player();
  
public:

  Camera3D* get_player_camera() { return m_PlayerCamera; }

  Node3D* get_player_head() { return m_PlayerHead; }
  Node3D* get_camera_controller() { return m_CameraControllerNode; }

  RayCast3D* get_collider_raycast() { return m_ColliderRayCast; }

  CollisionShape3D* get_player_standing_collider() { return m_StandingPlayerCollider; }
  CollisionShape3D* get_player_crouching_collider() { return m_CrouchingPlayerCollider; }

  Vector3 get_wish_dir() { return m_WishDir; }
  Vector3 get_gravity_vec() { return m_GravityVec; }
  
  void set_gravity_vec(Vector3 gravity_vec) { m_GravityVec = gravity_vec; }

  Vector2 get_input_dir() { return m_InputDir; }

  PlayerGlobalState& get_global_state() { return m_GlobalState; }

  void _update_input();
  void _update_velocity();

private:
  PlayerGlobalState m_GlobalState;

  Node3D* m_PlayerHead { nullptr };
  Node3D* m_CameraControllerNode { nullptr };
  
  // Get Collision shapes
  CollisionShape3D* m_StandingPlayerCollider { nullptr };
  CollisionShape3D* m_CrouchingPlayerCollider { nullptr };
  
  RayCast3D* m_ColliderRayCast { nullptr };
  Camera3D* m_PlayerCamera { nullptr };
  
  Node3D* m_CamController { nullptr };

  GlobalStateHandler* m_GlobalStateHandler { nullptr };
  Node3D* m_WeaponHoldPoint { nullptr };


private:

  // Player vectors & Input vectors
  Vector2 m_InputDir { Vector2(0.0f, 0.0f) };
  Vector3 m_WishDir { Vector3(0.0f, 0.0f, 0.0f) };
  Vector3 m_PlayerTiltVector { Vector3(0.0f, 0.0f, 0.0f) };

  Vector3 m_GravityVec = { Vector3(0.0f, 0.0f, 0.0f) };

private:
  GD_DEFINE_PROPERTY(float, crouch_speed, 3.0f);
  GD_DEFINE_PROPERTY(float, sprint_speed, 10.0f);
  GD_DEFINE_PROPERTY(float, dash_speed, 50.0f);

  GD_DEFINE_PROPERTY(float, crouch_translate, 0.8f);
  GD_DEFINE_PROPERTY(float, crouch_translate_speed, 10.0f);
  
  GD_DEFINE_PROPERTY(float, slide_speed, 10.0f);
  GD_DEFINE_PROPERTY(float, slide_timer, 2.0f);

  GD_DEFINE_PROPERTY(float, jump_height, 10.0f);
  GD_DEFINE_PROPERTY(float, down_gravity, 15.0f);

  GD_DEFINE_PROPERTY(float, max_air_move_speed, 10.0f);

  GD_DEFINE_PROPERTY(float, mouse_sensitivity, 0.003f);

  GD_DEFINE_PROPERTY(float, dash_cooldown, 1.0f);
  GD_DEFINE_PROPERTY(float, ground_accel, 0.0f);
  GD_DEFINE_PROPERTY(float, ground_decel, 0.2f);
};
