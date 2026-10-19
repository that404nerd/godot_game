#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physical_bone_simulator3d.hpp>
#include <godot_cpp/classes/physical_bone3d.hpp>

#include "../globals.h"

using namespace godot;

class HitBoxComponent : public Node3D 
{
  GDCLASS(HitBoxComponent, Node3D);
public:
  void _ready() override;

  void _on_hit(float totalDamage, Node* hitSource, Node* hitTarget);

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(CollisionShape3D*, hitboxCollider, nullptr);
  GD_DEFINE_PROPERTY(float, damage_multiplier, 1.0f);
};