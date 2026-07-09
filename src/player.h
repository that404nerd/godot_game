#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include "components/character_component.h"

#include "globals.h"
#include "weapon_manager.h"

using namespace godot;

class Player : public CharacterComponent {
  GDCLASS(Player, CharacterComponent);

protected:
  static void _bind_methods();

public:
  Player();
  
  void _ready() override;
  void _process(double delta) override;
  void _physics_process(double delta) override;
  void _unhandled_input(const Ref<InputEvent>& event) override;
  
};
