#pragma once

#include <godot_cpp/godot.hpp>

#include "player_state_machine.h"
#include "player.h"

class PlayerDashState : public State {

public:
  PlayerDashState(PlayerStateMachine* playerStateMachine)
    : State(static_cast<uint8_t>(PlayerStates::DASH)),
      m_PlayerStateMachine(playerStateMachine)
  {}; 

  virtual void _enter() override;
  virtual void _handle_input(const Ref<InputEvent>& event) override;
  virtual void _physics_update(double delta) override;

  virtual void _exit() override;

private:
  Player* m_PlayerInst { nullptr };
  PlayerStateMachine* m_PlayerStateMachine { nullptr };

  Vector3 m_DashDir;
};
