#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/skeleton_modifier3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include "godot_cpp/classes/physical_bone3d.hpp"
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>

#include "../../dd3d_cpp_api.hpp"

#include "../../player.h"
#include "../../singletons/event_bus.h"
#include "detection_area_component.h"
#include "godot_cpp/classes/physical_bone_simulator3d.hpp"

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
  void _update_component_transform();

  void _on_body_entered_inner_area(Node3D* body);
  void _on_body_exited_inner_area(Node3D* body);

public:

  void _physics_update(double delta);
  void _update(double delta);

  void _exit_tree() override;

public:
  bool can_see_player() { return m_CanSeePlayer; }

private:
  bool m_CanSeePlayer { false };
  bool m_DidEnterInnerArea { false };

  float m_RememberTime { 0.0f };

  Player* m_PlayerInst { nullptr };
  Marker3D* m_PlayerMarker { nullptr };

  PhysicsDirectSpaceState3D* m_SpaceState { nullptr };
  Ref<PhysicsRayQueryParameters3D> m_Query { nullptr };

  Dictionary m_CollisionResult;
  Array m_BoneRIDs {};

private:
  GD_DEFINE_PROPERTY(AICharacterComponent*, ai_character_component, nullptr);
  GD_DEFINE_PROPERTY(DetectionAreaComponent*, inner_detection_area, nullptr);
  GD_DEFINE_PROPERTY(PhysicalBoneSimulator3D*, ragdoll_simulator, nullptr);
  GD_DEFINE_PROPERTY(Marker3D*, vision_marker, nullptr);

  GD_DEFINE_PROPERTY(bool, can_debug, false);

  GD_DEFINE_PROPERTY(float, eye_fov, 60.0f);
  GD_DEFINE_PROPERTY(float, eye_raycast_ray_rot, 10.0f);
  GD_DEFINE_PROPERTY(float, eye_raycast_length, 500.0f);
  GD_DEFINE_PROPERTY(float, max_remember_time, 10.0f); // Forget time in seconds
};