#include "player_idle_state.h"

void PlayerIdleState::_enter()
{ 
  m_PlayerInst = m_PlayerStateMachine->get_player_inst();
}

void PlayerIdleState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump") && m_PlayerInst->is_on_floor()) {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::JUMP));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_PlayerInst->is_on_floor())
  {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::CROUCH));
  }

}

void PlayerIdleState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();
  m_PlayerInst->_update_velocity();
  
  Vector3 playerVel = m_PlayerInst->get_velocity();

  if(m_PlayerInst->get_input_dir() != Vector2(0.0f, 0.0f) && m_PlayerInst->is_on_floor()) {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::SPRINT));
  }

  if(playerVel.y < -1.0f || !m_PlayerInst->is_on_floor()) {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::FALL));
  }
}


void PlayerIdleState::_exit() {}; 
