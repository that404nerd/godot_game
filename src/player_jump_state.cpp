#include "player_jump_state.h"

void PlayerJumpState::_enter()
{ 
  m_PlayerInst = m_PlayerStateMachine->get_player_inst();
  
  _jump();
}

void PlayerJumpState::_handle_input(const Ref<InputEvent>& event) 
{
    
}

void PlayerJumpState::_jump()
{
  Vector3 playerVel = m_PlayerInst->get_velocity();

  playerVel.y = m_PlayerInst->get_jump_height();
  
  Vector3 gravity_vec = (m_PlayerInst->get_floor_normal() + Vector3(0.0f, 1.0f, 0.0f)).normalized() * m_PlayerInst->get_jump_height();
  m_PlayerInst->set_gravity_vec(gravity_vec);

  m_PlayerInst->set_velocity(playerVel);
}

void PlayerJumpState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();
  m_PlayerInst->_update_velocity();

  Vector3 playerVel = m_PlayerInst->get_velocity();

  if(playerVel.y < -1.0f || !m_PlayerInst->is_on_floor()) {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::FALL));
  }
  
  if (m_PlayerInst->is_on_floor()) {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::IDLE));
  }
}


void PlayerJumpState::_exit() 
{
  m_PlayerInst->set_gravity_vec(Vector3(0.0f, 0.0f, 0.0f));
}
