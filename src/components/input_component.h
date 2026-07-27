#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>

#include "../utils/command_system.h"

using namespace godot;

class InputComponent : public Node, public CommandSystem
{
  GDCLASS(InputComponent, Node);

public:

  InputComponent();

  void _ready() override;
  void _unhandled_input(const Ref<InputEvent>& event) override;

protected:
  static void _bind_methods();

private:
  CommandData& m_CmdData;
};