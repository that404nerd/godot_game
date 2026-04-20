#include "player_sprint_state.h"

void PlayerSprintState::_enter()
{ 
  m_PlayerInst = m_PlayerStateMachine->get_player_inst();
}

void PlayerSprintState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump")) {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::JUMP));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_PlayerInst->is_on_floor())
  {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::CROUCH));
  }
  
  // TODO: Have a direction enum or smtg like that
  if(m_PlayerInst->get_velocity().length() > (m_PlayerInst->get_sprint_speed() * 0.8f) && Input::get_singleton()->is_action_just_pressed("crouch"))
  {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::SLIDE));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("dash") && m_PlayerInst->get_global_state().CanDash)
  {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::DASH));
  }

}

void PlayerSprintState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();
  m_PlayerInst->_update_velocity();
  
  Vector3 playerVel = m_PlayerInst->get_velocity();
  playerVel = Utils::exp_decay(playerVel, m_PlayerInst->get_sprint_speed() * m_PlayerInst->get_wish_dir(), 15.0f, (float)delta);


  if(m_PlayerInst->get_global_state().DashCooldown <= 0.0f)
  {
    m_PlayerInst->get_global_state().CanDash = true;
    m_PlayerInst->get_global_state().DashCooldown = m_PlayerInst->get_dash_cooldown();
  }

  m_PlayerInst->set_velocity(playerVel);

  if(m_PlayerInst->get_velocity().length() < 1.0f && m_PlayerInst->is_on_floor()) {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::IDLE));
  }

  if(playerVel.y < -1.0f || !m_PlayerInst->is_on_floor()) {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::FALL));
  }
}

void PlayerSprintState::_exit() 
{
}
