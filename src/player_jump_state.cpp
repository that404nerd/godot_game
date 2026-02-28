#include "player_jump_state.h"

void PlayerJumpState::_enter()
{ 
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
  
  _jump();
}

void PlayerJumpState::_bind_methods()
{
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
  m_PlayerInst->get_global_state().JumpBufferCooldown = 0.0f;
}

void PlayerJumpState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();
  m_PlayerInst->_update_velocity();

  Vector3 playerVel = m_PlayerInst->get_velocity();

  if(playerVel.y < -1.0f || !m_PlayerInst->is_on_floor()) {
    emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::FALL));
  }
  
  if (m_PlayerInst->is_on_floor()) {
    emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::IDLE));
  }
}


void PlayerJumpState::_exit() 
{
  m_PlayerInst->set_gravity_vec(Vector3(0.0f, 0.0f, 0.0f));
}
