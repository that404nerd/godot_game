#pragma once

#include "state.h"
#include "state_machine.h"
#include "player.h"

class PlayerJumpState : public State {
  GDCLASS(PlayerJumpState, State);

public:
  PlayerJumpState() : m_PlayerInst(nullptr) {}; 

  virtual void _enter() override;
  virtual void _handle_input(const Ref<InputEvent>& event) override;
  virtual void _physics_update(double delta) override;
  virtual void _exit() override; 

  void _jump(); 

protected:
  static void _bind_methods();

private:
  Player* m_PlayerInst;
  StateMachine* m_StateMachineInst;
};
