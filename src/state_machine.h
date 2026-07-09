#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/templates/a_hash_map.hpp>
#include <godot_cpp/variant/string.hpp>

#include <memory>
#include <unordered_map>

#include "states/state.h"
#include "globals.h"

using namespace godot;

class StateMachine : public Node {
  
  GDCLASS(StateMachine, Node);
  
public:
  void _ready() override;

  virtual void _init_data() {};
  virtual void _handle_state_machine_input(const Ref<InputEvent>& event) {};
  virtual void _handle_state_machine_process(double delta) {};

  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _physics_process(double delta) override;
  void _process(double delta) override;

  void _change_state(int8_t stateID);

  int8_t get_current_state();
  int8_t get_prev_state();

protected:
  static void _bind_methods();

protected:
  State* m_CurrentState { nullptr };
  State* m_PrevState { nullptr };
  State* m_InitialState { nullptr };
  std::unordered_map<int8_t, std::unique_ptr<State>> m_States;
};
