#include "player_falling_state.h"
#include <godot_cpp/core/math.hpp>

void PlayerFallingState::_enter()
{ 
  m_StateMachine = GameManager::get_singleton()->get_player_state_machine();
  m_PlayerInst = m_StateMachine->get_player_inst();
}

void PlayerFallingState::_bind_methods()
{

}

void PlayerFallingState::_handle_input(const Ref<InputEvent>& event) 
{
  if (!m_IsJumpPressed && Input::get_singleton()->is_action_just_pressed("jump")) {
    emit_signal("state_changed", m_PlayerInst->SetCurrentState(Player::StateNames::JUMP));
    m_IsJumpPressed = true;
  }
}

void PlayerFallingState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();
  m_PlayerInst->_update_velocity();

  Vector3 playerVel = m_PlayerInst->get_velocity();
  Vector3 wish = m_PlayerInst->get_wish_dir();
  
  playerVel.y -= 12.0f * delta; // Special falling velocity

  float currentSpeed = playerVel.dot(Vector3(wish.x, 0, wish.z));
  float addSpeed = m_PlayerInst->get_max_air_move_speed() - currentSpeed;
  if (addSpeed > 0.0f) {
      playerVel.x += wish.x * m_PlayerInst->get_max_air_accel() * addSpeed * delta;
      playerVel.z += wish.z * m_PlayerInst->get_max_air_accel() * addSpeed * delta;
  }

  m_PlayerInst->set_velocity(playerVel);

  // if(Input::get_singleton()->is_action_just_pressed("dash"))
  // {
  //   emit_signal("state_changed", "Dash");
  // }
  
  if(m_PlayerInst->is_on_floor())
  {
      m_IsJumpPressed = false;
      emit_signal("state_changed", m_PlayerInst->SetCurrentState(Player::StateNames::IDLE));
  }
}


void PlayerFallingState::_exit() 
{
}
