#include "player_falling_state.h"

void PlayerFallingState::_enter()
{ 
  m_PlayerInst = m_PlayerStateMachine->get_player_inst();
}

void PlayerFallingState::_handle_input(const Ref<InputEvent>& event) 
{
  if (!m_IsJumpPressed && Input::get_singleton()->is_action_just_pressed("jump")) 
  {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::JUMP));
    m_IsJumpPressed = true;
  }

  if(Input::get_singleton()->is_action_just_pressed("dash") && m_PlayerInst->get_global_state().CanDash)
  {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::DASH));
  }

  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_IsCrouchPressed == false)
  {
    m_IsCrouchPressed = true;
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

  if(m_PlayerInst->get_global_state().DashCooldown <= 0.0f)
  {
    m_PlayerInst->get_global_state().CanDash = true;
    m_PlayerInst->get_global_state().DashCooldown = m_PlayerInst->get_dash_cooldown();
  }

  m_PlayerInst->set_velocity(playerVel);

  if(m_PlayerInst->is_on_floor())
  {
    if(m_IsCrouchPressed == true)
    {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::CROUCH));
    } else {
      m_IsJumpPressed = false;
      m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::IDLE));
    }
  }

}


void PlayerFallingState::_exit() 
{
  m_PlayerInst->set_gravity_vec(Vector3(0.0f, 0.0f, 0.0f));
  m_IsCrouchPressed = false;
}
