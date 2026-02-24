#include "player_falling_state.h"

void PlayerFallingState::_enter()
{ 
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();

}

void PlayerFallingState::_bind_methods()
{
}

void PlayerFallingState::_handle_input(const Ref<InputEvent>& event) 
{
  // if (!m_IsJumpPressed && Input::get_singleton()->is_action_just_pressed("jump")) 
  // {
  //   emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::JUMP));
  //   m_IsJumpPressed = true;
  // }
  //
}

void PlayerFallingState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();
  m_PlayerInst->_update_velocity();

  Vector3 playerVel = m_PlayerInst->get_velocity();
  Vector3 wishDir = m_PlayerInst->get_wish_dir();

  if(m_PlayerInst->get_global_state().JumpBufferCooldown > 0.0f)
  { 
    playerVel.y = m_PlayerInst->get_jump_height();
    m_PlayerInst->set_velocity(playerVel);
  }

  print_line(m_PlayerInst->get_global_state().JumpBufferCooldown);

  playerVel.y -= m_PlayerInst->get_down_gravity() * delta; // Special falling velocity

  float currentSpeed = playerVel.dot(wishDir);
  float addSpeed = m_PlayerInst->get_max_air_move_speed() - currentSpeed;

  float accel = m_PlayerInst->get_max_air_accel() * delta;

  if(accel > addSpeed)
    accel = addSpeed;
  
  if(addSpeed > 0.0f)
  {
    playerVel.x += wishDir.x * accel;
    playerVel.z += wishDir.z * accel;
  }

  m_PlayerInst->set_velocity(playerVel);



  if(m_PlayerInst->is_on_floor())
  {
    m_IsJumpPressed = false;
    emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::IDLE));
  }
}


void PlayerFallingState::_exit() 
{
}
