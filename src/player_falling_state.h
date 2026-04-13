#pragma once

#include "state.h"
#include "state_machine.h"
#include "player.h"

class PlayerFallingState : public State {
  GDCLASS(PlayerFallingState, State);

public:
  PlayerFallingState() : m_IsJumpPressed(false) {}; 

  virtual void _enter() override;
  virtual void _handle_input(const Ref<InputEvent>& event) override;
  virtual void _physics_update(double delta) override;
  virtual void _exit() override; 

protected:
  static void _bind_methods();

private:
  Player* m_PlayerInst;
  StateMachine* m_StateMachineInst;

  bool m_IsJumpPressed, m_IsCrouchPressed;
};
