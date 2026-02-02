#include "player_jump_state.h"

void PlayerJumpState::_enter()
{ 
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();

  Vector3 playerVel = m_PlayerInst->get_velocity();
  playerVel.y = m_PlayerInst->get_jump_height();
  m_PlayerInst->set_velocity(playerVel);
}

void PlayerJumpState::_bind_methods()
{
}

void PlayerJumpState::_handle_input(const Ref<InputEvent>& event) 
{
    
}

void PlayerJumpState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();
  m_PlayerInst->_update_velocity();
  
  Vector3 playerVel = m_PlayerInst->get_velocity();

  playerVel.y -= 12.0f * delta;

  m_PlayerInst->set_velocity(playerVel);
  
  if(playerVel.y < -1.0f || !m_PlayerInst->is_on_floor()) {
    emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::FALL));
  }
  
  if (m_PlayerInst->is_on_floor()) {
      emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::IDLE));
  }
}


void PlayerJumpState::_exit() {
}
