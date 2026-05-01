#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/character_body3d.hpp>

#include "../globals.h"

using namespace godot;

class CharacterComponent : public Node {
  GDCLASS(CharacterComponent, Node)

public:
  void _ready() override
  {
    if (!character_body_path.is_empty() && has_node(character_body_path)) {
      Node* node = get_node<CharacterBody3D>(character_body_path);
      character_body = Object::cast_to<CharacterBody3D>(node);
    }
  }

protected:
  static void _bind_methods()
  {
    GD_BIND_CUSTOM_PROPERTY(CharacterComponent, character_body_path, Variant::NODE_PATH, PROPERTY_HINT_NONE);
  }

private:
  GD_DEFINE_PROPERTY(CharacterBody3D*, character_body, nullptr);
  GD_DEFINE_PROPERTY(NodePath, character_body_path, NodePath());
};