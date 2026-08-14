#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>

#include <godot_cpp/classes/scene_tree.hpp>

#include "../../dd3d_cpp_api.hpp"

#include "../../player.h"

class AICharacterComponent;

using namespace godot;

class VisionComponent : public Node3D
{
  GDCLASS(VisionComponent, Node3D);

protected:
  static void _bind_methods();

public:
  void _init();

  void _update_component_transform();

  void _physics_update(double delta);
  void _update(double delta);

private:
  Transform3D m_BoneTransform {};
  Vector3 m_BoneRot {}, m_ForwardVector {}, m_PlayerPos {};

  Player* m_PlayerInst { nullptr };

  GD_DEFINE_PROPERTY(AICharacterComponent*, ai_character_component, nullptr);
  GD_DEFINE_PROPERTY(Skeleton3D*, character_skeleton, nullptr);
  GD_DEFINE_PROPERTY(StringName, character_bone_name, StringName());
};