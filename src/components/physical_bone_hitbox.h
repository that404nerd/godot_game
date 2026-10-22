#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/physical_bone3d.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "./hitbox_component.h"
#include "../globals.h"

using namespace godot;

class PhysicalBoneHitbox : public PhysicalBone3D
{
  GDCLASS(PhysicalBoneHitbox, PhysicalBone3D);
public:
  void _ready() override;

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(HitBoxComponent*, boneHitBox, nullptr);
};