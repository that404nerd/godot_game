#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

#include "../globals.h"
#include "../singletons/event_bus.h"

using namespace godot;

class HealthComponent : public Node
{
  GDCLASS(HealthComponent, Node);

public:
  void _ready() override;

  void _take_damage(float damage);
  float get_current_health() { return m_CurrentHealth; }

protected:
  static void _bind_methods();
  
private:
  GD_DEFINE_PROPERTY(float, max_health, 0);
  float m_CurrentHealth { 0 };
};