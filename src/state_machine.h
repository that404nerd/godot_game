#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/templates/a_hash_map.hpp>
#include <godot_cpp/variant/string.hpp>

#include "global_state_handler.h"
#include "state.h"
#include "player.h"
#include "game_manager.h"
#include "globals.h"
#include "weapon_manager.h"

using namespace godot;

class StateMachine : public Node {
  GDCLASS(StateMachine, Node);

public:
  StateMachine();

  void _ready() override;

  void _input(const Ref<InputEvent>& event) override;
  void _physics_process(double delta) override;

  void _change_state(const String& stateName);

  StringName get_current_state();
  
  StringName get_prev_state();

  ~StateMachine();

protected:
  static void _bind_methods();
private:
  GD_DEFINE_PROPERTY(State*, initial_state, nullptr); 

  GlobalStateHandler* m_GlobalStateHandlerInst = nullptr;

  State* m_CurrentState { nullptr };
  State* m_PrevState { nullptr };

  AHashMap<String, State*> m_States;
};
