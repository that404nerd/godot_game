#pragma once

#include "state.h"
#include "player_state_machine.h"
#include "player.h"

class PlayerJumpState : public State {

public:
  PlayerJumpState(PlayerStateMachine* playerStateMachine) : 
    State(static_cast<int8_t>(PlayerStates::JUMP)), 
    m_PlayerStateMachine(playerStateMachine) {};

  virtual void _enter() override;
  virtual void _handle_input(const Ref<InputEvent>& event) override;
  virtual void _physics_update(double delta) override;
  virtual void _exit() override; 

  void _jump(); 

private:
  PlayerStateMachine* m_PlayerStateMachine { nullptr };
  Player* m_PlayerInst { nullptr };
};
