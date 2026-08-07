#pragma once

#include <godot_cpp/godot.hpp>

#include "character_component.h"

class AIStateMachine;

class AICharacterComponent : public CharacterComponent
{
  GDCLASS(AICharacterComponent, CharacterComponent);

protected:
  static void _bind_methods();

public:
  void _ready() override;
  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _process(double delta) override;
  void _physics_process(double delta) override;

private:
  GD_DEFINE_PROPERTY(AIStateMachine*, ai_state_machine, nullptr);
};