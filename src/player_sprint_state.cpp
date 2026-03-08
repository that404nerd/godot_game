#include "player_sprint_state.h"
#include "player_state_machine.h"

void PlayerSprintState::_enter()
{ 
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  
}

void PlayerSprintState::_bind_methods()
{
}

void PlayerSprintState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump")) {
    emit_signal("state_changed", "Jump");
  }
  
  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_PlayerInst->is_on_floor())
  {
    emit_signal("state_changed", "Crouch");
  }
  
  // TODO: Have a direction enum or smtg like that
  if(m_PlayerInst->get_velocity().length() > (m_PlayerInst->get_sprint_speed() * 0.8f) && Input::get_singleton()->is_action_just_pressed("crouch"))
  {
    emit_signal("state_changed", "Slide");
  }
  
  if(Input::get_singleton()->is_action_just_pressed("dash"))
  {
    emit_signal("state_changed", "Dash");
  }

}

void PlayerSprintState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();
  m_PlayerInst->_update_velocity();
  
  Vector3 playerVel = m_PlayerInst->get_velocity();
  playerVel = Utils::exp_decay(playerVel, m_PlayerInst->get_sprint_speed() * m_PlayerInst->get_wish_dir(), 15.0f, (float)delta);

  m_PlayerInst->set_velocity(playerVel);

  if(m_PlayerInst->get_velocity().length() < 1.0f && m_PlayerInst->is_on_floor()) {
    emit_signal("state_changed", "Idle");
  }

  if(playerVel.y < -1.0f || !m_PlayerInst->is_on_floor()) {
    emit_signal("state_changed", "Fall");
  }
}

void PlayerSprintState::_exit() 
{
}
