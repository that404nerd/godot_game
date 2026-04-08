#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/character_body3d.hpp>

#include <typeinfo>

#include "../globals.h"
#include "godot_cpp/core/type_info.hpp"

using namespace godot;

class CharacterComponent : public Node {
  GDCLASS(CharacterComponent, Node)

protected:
  static void _bind_methods()
  {
  }

public:
  void _ready() override
  {
    m_CharacterBody = Object::cast_to<CharacterBody3D>(get_parent());
  }

  CharacterBody3D* get_character_body() { 

    if(m_CharacterBody != nullptr)
    {
      return m_CharacterBody;
      print_error("Character Body instance set successfully!");
    }
    
    return nullptr;
  }

private:
  CharacterBody3D* m_CharacterBody = nullptr;
};
