#include "player_dash_state.h"
#include "player.h"

void PlayerDashState::_enter()
{ 
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  
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

  m_PlayerInst->get_global_state().CanDash = false;

  Vector3 playerVel = m_PlayerInst->get_velocity();

  playerVel.x = Math::abs(m_DashDir.x) * playerVel.x * 7.0f;
  playerVel.z = Math::abs(m_DashDir.z) * playerVel.z * 7.0f;
  
  m_PlayerInst->set_velocity(playerVel);

  if(m_PlayerInst->is_on_floor())
  {
    emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::SPRINT));
  }
  
  if(playerVel.y < -1.0f || !m_PlayerInst->is_on_floor()) 
  {
    emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::FALL));
  }

}

void PlayerDashState::_exit() 
{

}
