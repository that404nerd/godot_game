#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/global_constants.hpp>

#include "../globals.h"

using namespace godot;

class CharacterComponent : public CharacterBody3D {
  GDCLASS(CharacterComponent, CharacterBody3D)

public:
  Vector3 get_wish_dir() { return m_WishDir; }
  Vector2 get_input_dir() { return m_InputDir; }

public:
  void _update_input(double delta) 
  {
    Vector3 characterVel = get_velocity();

    if(!is_on_floor())
    {
      characterVel.y -= down_gravity * delta;
    }

    m_InputDir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized();
    m_WishDir = get_global_transform().basis.xform(Vector3(m_InputDir.x, 0.0f, m_InputDir.y)).normalized();
    
    if (is_on_floor())
    {
      if (m_WishDir != Vector3(0.0f, 0.0f, 0.0f))
      {
        characterVel.x = Utils::exp_decay(characterVel.x, m_WishDir.x, 15.0f, ground_accel);
        characterVel.z = Utils::exp_decay(characterVel.z, m_WishDir.z, 15.0f, ground_accel);
      }
      else
      {
        characterVel.x = Utils::exp_decay(characterVel.x, 0.0f, 1.0f, ground_decel);
        characterVel.z = Utils::exp_decay(characterVel.z, 0.0f, 1.0f, ground_decel);
      }
    }

    set_velocity(characterVel);
  }

  void _update_velocity()
  {
    move_and_slide();
  }


protected:
  static void _bind_methods()
  {
    ADD_GROUP("Character Nodes", "");
    GD_BIND_CUSTOM_PROPERTY(CharacterComponent, character_head, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(CharacterComponent, crouch_raycast, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(CharacterComponent, default_collision_shape, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(CharacterComponent, crouch_collision_shape, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    
    ADD_GROUP("Speed Settings", "");
    GD_BIND_PROPERTY(CharacterComponent, crouch_speed, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, sprint_speed, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, slide_speed, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, dash_speed, Variant::FLOAT);

    ADD_GROUP("Jump Settings", "");
    GD_BIND_PROPERTY(CharacterComponent, jump_height, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, down_gravity, Variant::FLOAT);

    ADD_GROUP("Crouch Settings", "");
    GD_BIND_PROPERTY(CharacterComponent, crouch_translate, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, crouch_ang_freq, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, crouch_damping_ratio, Variant::FLOAT);

    ADD_GROUP("Slide Settings", "");
    GD_BIND_PROPERTY(CharacterComponent, slide_timer, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, slide_ang_freq, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, slide_damping_ratio, Variant::FLOAT);

    ADD_GROUP("Air Strafe Settings", "");
    GD_BIND_PROPERTY(CharacterComponent, max_air_move_speed, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, mouse_sensitivity, Variant::FLOAT);

    ADD_GROUP("Acceleration Settings", "");
    GD_BIND_PROPERTY(CharacterComponent, ground_accel, Variant::FLOAT);
    GD_BIND_PROPERTY(CharacterComponent, ground_decel, Variant::FLOAT);

    ADD_GROUP("Misc Settings", "");
    GD_BIND_PROPERTY(CharacterComponent, dash_cooldown, Variant::FLOAT);
  }

private:
  Vector2 m_InputDir { Vector2(0.0f, 0.0f) };
  Vector3 m_WishDir { Vector3(0.0f, 0.0f, 0.0f) };


// These are the main settings that change how the character moves
private:
  GD_DEFINE_PROPERTY(Node3D*, character_head, nullptr);
  GD_DEFINE_PROPERTY(RayCast3D*, crouch_raycast, nullptr);
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
