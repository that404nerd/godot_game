#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class State {
public:

  State(uint8_t stateID) : m_StateId(stateID) {};

  virtual void _enter() = 0;
  virtual void _handle_input(const Ref<InputEvent>& event) = 0;
  virtual void _physics_update(double delta) {};
  virtual void _update(double delta) {};

  virtual void _exit() = 0;
  virtual ~State() {};

public:
  uint8_t get_current_state() { return m_StateId; }

private:
  uint8_t m_StateId;
};
