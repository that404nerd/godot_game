#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/shape_cast3d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>

#include "../../dd3d_cpp_api.hpp"

#include "../../player.h"
#include "../../singletons/event_bus.h"

class AICharacterComponent;

using namespace godot;

class VisionComponent : public Node3D
{
  GDCLASS(VisionComponent, Node3D);

protected:
  static void _bind_methods();

public:
  void _init();

private:
  void _setup_vision_raycasts();
  void _update_component_transform();

public:
  void _physics_update(double delta);
  void _update(double delta);

  void _exit_tree() override;

public:
  bool can_see_player() { return m_SawPlayer; }

private:
  bool m_SawPlayer { false }, m_Colliding { false };
  float m_RememberTime { 0.0f };

  Transform3D m_BoneTransform {};
  Vector3 m_BoneRot {}, m_ForwardVector {}, m_PlayerPos {};

  Array m_VisionRayCasts { nullptr };
  Ref<RandomNumberGenerator> m_RngGen { nullptr };

  Player* m_PlayerInst { nullptr };

private:

  GD_DEFINE_PROPERTY(AICharacterComponent*, ai_character_component, nullptr);
  GD_DEFINE_PROPERTY(Skeleton3D*, character_skeleton, nullptr);
  GD_DEFINE_PROPERTY(StringName, character_bone_name, StringName());
  GD_DEFINE_PROPERTY(ShapeCast3D*, viewable_area, nullptr);

  GD_DEFINE_PROPERTY(float, eye_raycast_ray_rot, 10.0f);
  GD_DEFINE_PROPERTY(float, eye_raycast_length, 14.0f);
  GD_DEFINE_PROPERTY(int, eye_raycasts_count, 16);
  GD_DEFINE_PROPERTY(float, max_remember_time, 10.0f); // Forget time in seconds
};