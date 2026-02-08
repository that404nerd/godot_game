#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/templates/a_hash_map.hpp>
#include <godot_cpp/variant/string.hpp>

#include "player_state.h"
#include "player.h"
#include "game_manager.h"
#include "globals.h"

using namespace godot;

class PlayerStateMachine : public Node {
  GDCLASS(PlayerStateMachine, Node);

public:
  PlayerStateMachine();

  void _ready() override;

  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _physics_process(double delta) override;

  void _change_state(const String& stateName);
  StringName get_current_state();
  
  Player* get_player_inst() const { return m_PlayerInst; }
  StringName get_prev_state();

  ~PlayerStateMachine();

public:
  enum class StateNames {
    IDLE, SPRINT, CROUCH, JUMP, DASH, FALL, SLIDE
  };

  inline StringName GetCurrentState(StateNames state)
  {
    StringName currentState;
    switch(state)
    {
      case StateNames::IDLE:
        currentState = "Idle";
        break;
      case StateNames::SPRINT:
        currentState = "Sprint";
        break;
      case StateNames::JUMP:
        currentState = "Jump";
        break;
      case StateNames::FALL:
        currentState = "Fall";
        break;
      case StateNames::DASH:
        currentState = "Dash";
        break;
      case StateNames::CROUCH:
        currentState = "Crouch";
        break;
      case StateNames::SLIDE:
        currentState = "Slide";
        break;
    }

    return currentState;
  }

protected:
  static void _bind_methods();
private:
  GD_DEFINE_PROPERTY(PlayerState*, initial_state, nullptr); 

  Player* m_PlayerInst { nullptr };
  PlayerState* m_CurrentState { nullptr };
  PlayerState* m_PrevState { nullptr };
  AHashMap<String, PlayerState*> m_States;
};
