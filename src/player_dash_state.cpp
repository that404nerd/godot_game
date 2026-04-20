#include "player_dash_state.h"

void PlayerDashState::_enter()
{ 
  m_PlayerInst = m_PlayerStateMachine->get_player_inst();

  m_DashDir = m_PlayerInst->get_wish_dir();
}

void PlayerDashState::_handle_input(const Ref<InputEvent>& event) 
{
}

void PlayerDashState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();    
  m_PlayerInst->_update_velocity();

  m_PlayerInst->get_global_state().CanDash = false;
  
  Vector3 playerVel = m_PlayerInst->get_velocity();
  
  playerVel.x = m_DashDir.x * 50.0f;
  playerVel.z = m_DashDir.z * 50.0f;

  m_PlayerInst->set_velocity(playerVel);

  if(m_PlayerInst->get_velocity().length() > 0.0f)
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::SPRINT));
  else
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::IDLE));
  
}

void PlayerDashState::_exit() 
{
}
