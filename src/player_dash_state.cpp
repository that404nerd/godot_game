#include "player_dash_state.h"

void PlayerDashState::_enter()
{ 
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();

  m_DashDir = m_PlayerInst->get_wish_dir();
}

void PlayerDashState::_bind_methods()
{
}

void PlayerDashState::_handle_input(const Ref<InputEvent>& event) 
{
}

void PlayerDashState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();    
  m_PlayerInst->_update_velocity();
  
  Vector3 playerVel = m_PlayerInst->get_velocity();
  
  playerVel.x = m_DashDir.x * 50.0f;
  playerVel.z = m_DashDir.z * 50.0f;

  m_PlayerInst->set_velocity(playerVel);

  if(m_PlayerInst->get_velocity().length() > 0.0f)
    emit_signal("state_changed", "Sprint");
  else
    emit_signal("state_changed", "Idle");
  
}

void PlayerDashState::_exit() 
{
}
