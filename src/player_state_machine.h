#pragma once

#include "state_machine.h"

class PlayerIdleState;
class PlayerSprintState;
class PlayerJumpState;
class PlayerFallingState;
class PlayerCrouchState;
class PlayerSlideState;
class PlayerDashState;

enum class PlayerStates : uint8_t {
  IDLE, SPRINT, JUMP, FALL, CROUCH, SLIDE, DASH
};

class PlayerStateMachine : public StateMachine 
{
  GDCLASS(PlayerStateMachine, StateMachine);

public:
  void _init_data(Player* playerInst);
  Player* get_player_inst() const { return m_PlayerInst; }

protected:
  static void _bind_methods();

private:
  Player* m_PlayerInst;
};