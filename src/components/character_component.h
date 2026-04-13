#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/character_body3d.hpp>

#include "../globals.h"

using namespace godot;

class CharacterComponent {

public:

  CharacterBody3D* get_character_body()
  { 
    if(m_CharacterBody != nullptr)
    {
      print_line("Character Body instance set successfully!");
      return m_CharacterBody;
    }
    return nullptr;
  }

  void set_character_body_inst(CharacterBody3D* characterBody) { m_CharacterBody = characterBody; }

private:
  CharacterBody3D* m_CharacterBody { nullptr };
};
