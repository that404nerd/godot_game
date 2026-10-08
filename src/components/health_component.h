#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

#include "../globals.h"

using namespace godot;

class HealthComponent : public Node
{
  GDCLASS(HealthComponent, Node);

public:
  void _ready() override;

  void _take_damage(int damage);

protected:
  static void _bind_methods();
  
private:
  GD_DEFINE_PROPERTY(int, max_health, 0);
};