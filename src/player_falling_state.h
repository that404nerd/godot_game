#pragma once

#include "player_state_machine.h"
#include "state.h"
#include "player.h"

class PlayerFallingState : public State {

public:
  PlayerFallingState(PlayerStateMachine* playerStateMachine) : State(static_cast<int>(PlayerStates::FALL)), 
                                                                m_PlayerStateMachine(playerStateMachine) {};

  virtual void _enter() override;
  virtual void _handle_input(const Ref<InputEvent>& event) override;
  virtual void _physics_update(double delta) override;
  virtual void _exit() override; 

private:
  Player* m_PlayerInst { nullptr };
  PlayerStateMachine* m_PlayerStateMachine { nullptr };

  bool m_IsJumpPressed { false }, m_IsCrouchPressed;
};
