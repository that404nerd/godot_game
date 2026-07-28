#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>

#include "../input_command_system.h"

using namespace godot;

class InputComponent : public InputCommandSystem
{
  GDCLASS(InputComponent, InputCommandSystem);

public:

  InputComponent();

  void _ready() override;
  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _process(double delta) override;

protected:
  static void _bind_methods();

private:
  float m_HoldCounter { 0.0f };

  CommandData& m_CmdData;
};