#pragma once

#include "player_state_machine.h"
#include "state.h"
#include "state_machine.h"
#include "player.h"

#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

#include "globals.h"

#include "dd3d_cpp_api.hpp"

class PlayerSprintState : public State {

public:
  PlayerSprintState(PlayerStateMachine* playerStateMachine) 
    : State(static_cast<uint8_t>(PlayerStates::SPRINT)),
    m_PlayerStateMachine(playerStateMachine)
  {}; 

  virtual void _enter() override;
  virtual void _handle_input(const Ref<InputEvent>& event) override;
  virtual void _physics_update(double delta) override;
  virtual void _exit() override; 

  void _headbob_effect(double delta);

private:
  Player* m_PlayerInst;
  PlayerStateMachine* m_PlayerStateMachine { nullptr };
};
