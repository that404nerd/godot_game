#include "player_falling_state.h"
#include "globals.h"

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
  if (!m_IsJumpPressed && Input::get_singleton()->is_action_just_pressed("jump")) 
  {
    emit_signal("state_changed", "Jump");
    m_IsJumpPressed = true;
  }

  if(Input::get_singleton()->is_action_just_pressed("dash"))
  {
    emit_signal("state_changed", "Dash");
  }
}

void PlayerFallingState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();
  m_PlayerInst->_update_velocity();

  Vector3 playerVel = m_PlayerInst->get_velocity();
  Vector3 wishDir = m_PlayerInst->get_wish_dir().normalized();

  Vector3 gravity_vec = Vector3(0.0f, -1.0f, 0.0f) * m_PlayerInst->get_down_gravity() * delta;
  m_PlayerInst->set_gravity_vec(gravity_vec);
  
  float targetX = wishDir.x * m_PlayerInst->get_max_air_move_speed();
  float targetZ = wishDir.z * m_PlayerInst->get_max_air_move_speed();

  if (wishDir.length() > 0.0f) {
    playerVel.x = Utils::exp_decay(playerVel.x, targetX, 15.0f, delta);
    playerVel.z = Utils::exp_decay(playerVel.z, targetZ, 15.0f, delta);
  }

  m_PlayerInst->set_velocity(playerVel);

  if(m_PlayerInst->is_on_floor())
  {
    m_IsJumpPressed = false;
    emit_signal("state_changed", "Idle");
  }}


void PlayerFallingState::_exit() 
{
  m_PlayerInst->set_gravity_vec(Vector3(0.0f, 0.0f, 0.0f));
}
