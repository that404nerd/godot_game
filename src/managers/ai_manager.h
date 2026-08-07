#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>

using namespace godot;

class AIManager : public Node {
  GDCLASS(AIManager, Node);

protected:
  static void _bind_methods() {};
};