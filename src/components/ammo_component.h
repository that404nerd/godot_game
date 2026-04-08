#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>

#include "../globals.h"

using namespace godot;

class AmmoComponent : public Node {
  GDCLASS(AmmoComponent, Node)

public:

  void consume_ammo(int ammo)
  {

  }

protected:
  static void _bind_methods()
  {

  }

private:
  GD_DEFINE_PROPERTY(int, total_ammo, 0);
  GD_DEFINE_PROPERTY(int, current_ammo_count, 0);
};
