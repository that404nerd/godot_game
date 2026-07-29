#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

#include "../globals.h"
#include "input_component.h"
#include "weapon_component.h"

class WeaponEffects;
class MovementManager;
class MovementStateMachine;
class WeaponManager;
class WeaponStateMachine;

using namespace godot;

class CharacterComponent : public CharacterBody3D {
  GDCLASS(CharacterComponent, CharacterBody3D)

public:
  Vector3 get_wish_dir() { return m_WishDir; }
  Vector2 get_input_dir() { return m_InputDir; }

public:
  void _update_input(double delta);

  void _process(double delta) override;
  void _physics_process(double delta) override;
  
  void _update_velocity();

protected:
  static void _bind_methods();

private:
  Vector2 m_InputDir { Vector2(0.0f, 0.0f) };
  Vector3 m_WishDir { Vector3(0.0f, 0.0f, 0.0f) };


private:

  GD_DEFINE_PROPERTY(MovementManager*, movement_manager, nullptr);
  GD_DEFINE_PROPERTY(MovementStateMachine*, movement_state_machine, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponManager*, weapon_manager, nullptr);
  GD_DEFINE_PROPERTY(WeaponStateMachine*, weapon_state_machine, nullptr);
  GD_DEFINE_PROPERTY(InputCommandSystem*, input_command_system, nullptr);
  GD_DEFINE_PROPERTY(WeaponEffects*, weapon_effects_component, nullptr);

  GD_DEFINE_PROPERTY(Node3D*, character_head, nullptr);
  GD_DEFINE_PROPERTY(RayCast3D*, crouch_raycast, nullptr);
  GD_DEFINE_PROPERTY(RayCast3D*, stairs_below_raycast, nullptr);
  GD_DEFINE_PROPERTY(RayCast3D*, stairs_ahead_raycast, nullptr);
  GD_DEFINE_PROPERTY(CollisionShape3D*, default_collision_shape, nullptr);
  GD_DEFINE_PROPERTY(CollisionShape3D*, crouch_collision_shape, nullptr);

  GD_DEFINE_PROPERTY(float, crouch_speed, 3.0f);
  GD_DEFINE_PROPERTY(float, sprint_speed, 10.0f);
  GD_DEFINE_PROPERTY(float, dash_speed, 50.0f);

  GD_DEFINE_PROPERTY(float, crouch_translate, 0.5f);
  GD_DEFINE_PROPERTY(float, crouch_ang_freq, 30.0f);
  GD_DEFINE_PROPERTY(float, crouch_damping_ratio, 0.9f);
  
  GD_DEFINE_PROPERTY(float, slide_speed, 10.0f);
  GD_DEFINE_PROPERTY(float, slide_ang_freq, 35.0f);
  GD_DEFINE_PROPERTY(float, slide_damping_ratio, 0.3f);
  GD_DEFINE_PROPERTY(float, slide_timer, 2.0f);

  GD_DEFINE_PROPERTY(float, jump_height, 10.0f);
  GD_DEFINE_PROPERTY(float, down_gravity, 15.0f);

  GD_DEFINE_PROPERTY(float, max_air_move_speed, 10.0f);

  GD_DEFINE_PROPERTY(float, mouse_sensitivity, 0.003f);

  GD_DEFINE_PROPERTY(float, dash_cooldown, 1.0f);
  GD_DEFINE_PROPERTY(float, ground_accel, 0.0f);
  GD_DEFINE_PROPERTY(float, ground_decel, 0.2f);

};
