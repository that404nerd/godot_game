#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/node.hpp>

#include "../input_command_system.h"

using namespace godot;

class InputComponent : public InputCommandSystem
{
  GDCLASS(InputComponent, InputCommandSystem);

public:

  InputComponent();

  void _init() override;
  void _input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;

protected:
  static void _bind_methods();

private:
  float m_HoldCounter { 0.0f };

  InputCommandData& m_InputCmdData;
};