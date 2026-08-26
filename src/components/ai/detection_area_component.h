#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

#include "../../globals.h"

using namespace godot;

class DetectionAreaComponent : public Area3D
{
  GDCLASS(DetectionAreaComponent, Area3D);

public:
  Ref<Shape3D> get_collision_shape() { return detection_collision_shape->get_shape(); }

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(CollisionShape3D*, detection_collision_shape, nullptr);
};