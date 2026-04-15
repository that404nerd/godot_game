#include "player_state_machine.h"

#include "player_idle_state.h"
#include "player_sprint_state.h"
#include "player_jump_state.h"
#include "player_falling_state.h"
#include "player_crouch_state.h"
#include "player_slide_state.h"
#include "player_dash_state.h"

PlayerStateMachine::PlayerStateMachine()
{
  m_States[static_cast<uint8_t>(PlayerStates::IDLE)] = std::make_unique<PlayerIdleState>(this);
  m_States[static_cast<uint8_t>(PlayerStates::SPRINT)] = std::make_unique<PlayerSprintState>(this);
  m_States[static_cast<uint8_t>(PlayerStates::JUMP)] = std::make_unique<PlayerJumpState>(this);
  m_States[static_cast<uint8_t>(PlayerStates::FALL)] = std::make_unique<PlayerFallingState>(this);
  m_States[static_cast<uint8_t>(PlayerStates::CROUCH)] = std::make_unique<PlayerCrouchState>(this);
  m_States[static_cast<uint8_t>(PlayerStates::SLIDE)] = std::make_unique<PlayerSlideState>(this);
  m_States[static_cast<uint8_t>(PlayerStates::DASH)] = std::make_unique<PlayerDashState>(this);

  m_InitialState = m_States[0].get();
}

