#include "player_sprint_state.h"

void PlayerSprintState::_enter()
{ 
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  
  m_HoldPoint = m_PlayerInst->get_rig_hold_point();
}

void PlayerSprintState::_bind_methods()
{
  GD_BIND_PROPERTY(PlayerSprintState, headbob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(PlayerSprintState, headbob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(PlayerSprintState, headbob_delta_translate, Variant::FLOAT);

}

void PlayerSprintState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump")) {
      emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::JUMP));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_PlayerInst->is_on_floor())
  {
      emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::CROUCH));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("dash"))
  {
      emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::DASH));
  } 
}

void PlayerSprintState::_headbob_effect(double delta)
{
  bool onFloor = m_PlayerInst->is_on_floor(); // so that bobbing doesn't occur during airborne states

  float velocity = m_PlayerInst->get_velocity().length();
  m_HeadbobTime += delta * velocity * onFloor;

  float x_bob = Math::cos(m_HeadbobTime * headbob_freq * 0.5f) * headbob_amp;
  float y_bob = Math::sin(m_HeadbobTime * headbob_freq) * headbob_amp;

  Vector3 currentPos = m_PlayerInst->get_player_head()->get_position();
  Vector3 newPos = Vector3(
    Math::lerp(currentPos.x, x_bob, (float)delta * headbob_delta_translate),
    Math::lerp(currentPos.y, y_bob, (float)delta * headbob_delta_translate), 
    0.0f
  );

  m_PlayerInst->get_player_head()->set_position(newPos);

}

void PlayerSprintState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();    
  m_PlayerInst->_update_velocity();
  
  
  Vector3 playerVel = m_PlayerInst->get_velocity();
  float currentSpeedInWishDir = m_PlayerInst->get_velocity().dot(m_PlayerInst->get_wish_dir());
  float addSpeed = m_PlayerInst->get_sprint_speed() - currentSpeedInWishDir;

  Vector3 holdPointPos = m_HoldPoint->get_position();
  holdPointPos.x = Math::lerp(m_HoldPoint->get_position().x, 0.0f, (float)delta * 5.0f);
  holdPointPos.y = Math::lerp(m_HoldPoint->get_position().y, 0.0f, (float)delta * 5.0f);
  m_HoldPoint->set_position(holdPointPos);
 
  if(addSpeed > 0.0f) {
      float accel = m_PlayerInst->get_ground_accel() * m_PlayerInst->get_sprint_speed() * delta;
      accel = Math::min(accel, addSpeed);
      playerVel += accel * m_PlayerInst->get_wish_dir();
  } 

  
  // Friciton code
  float control = Math::max(playerVel.length(), m_PlayerInst->get_ground_decel()); // Dont let speed to drop to zero instead to ground decl when stopping
  float drop = control * m_PlayerInst->get_ground_friction() * delta; // how much velocity should be dropped due to friction
  float newSpeed = Math::max(playerVel.length() - drop, 0.0f); // New speed has to be subtracted from the current velocity due to friction
  
  if(playerVel.length() > 0.0f) {
      newSpeed /= playerVel.length();
  }
  
  playerVel *= newSpeed;
  _headbob_effect(delta);
  
  m_PlayerInst->set_velocity(playerVel);
  
  if(m_PlayerInst->get_input_dir() == Vector2(0.0f, 0.0f) && m_PlayerInst->is_on_floor()) {
      emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::IDLE));
  }

  if(playerVel.y < 1.0f && !m_PlayerInst->is_on_floor()) {
      emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::FALL));
  }

}

void PlayerSprintState::_exit() 
{
}
