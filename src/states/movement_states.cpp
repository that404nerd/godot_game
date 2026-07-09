#include "movement_states.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Idle Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void IdleMovementState::_enter()
{ 
  m_CharacterComp = m_MovementStateMachine->get_character_component();
}

void IdleMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump") && m_CharacterComp->is_on_floor()) {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::JUMP));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_CharacterComp->is_on_floor())
  {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::CROUCH));
  }

}

void IdleMovementState::_physics_update(double delta) 
{
  m_CharacterComp->_update_input();
  m_CharacterComp->_update_velocity();
  
  Vector3 playerVel = m_CharacterComp->get_velocity();

  if(m_CharacterComp->get_input_dir() != Vector2(0.0f, 0.0f) && m_CharacterComp->is_on_floor()) {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::SPRINT));
  }

  if(playerVel.y < -1.0f || !m_CharacterComp->is_on_floor()) {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::FALL));
  }
}


void IdleMovementState::_exit() {};


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Sprint Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void SprintMovementState::_enter()
{ 
  m_CharacterComp = m_MovementStateMachine->get_character_component();
}

void SprintMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump")) {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::JUMP));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_CharacterComp->is_on_floor())
  {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::CROUCH));
  }
  
  if(m_CharacterComp->get_velocity().length() > (m_CharacterComp->get_sprint_speed() * 0.8f) && Input::get_singleton()->is_action_just_pressed("crouch"))
  {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::SLIDE));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("dash") /* && m_CharacterComp->get_global_state().CanDash*/)
  {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::DASH));
  }

}

void SprintMovementState::_physics_update(double delta) 
{
  m_CharacterComp->_update_input();
  m_CharacterComp->_update_velocity();
  
  Vector3 playerVel = m_CharacterComp->get_velocity();
  playerVel =  Utils::exp_decay(playerVel, m_CharacterComp->get_sprint_speed() * m_CharacterComp->get_wish_dir(), 15.0f, (float)delta);


  // if(m_CharacterComp->get_global_state().DashCooldown <= 0.0f)
  // {
  //   m_CharacterComp->get_global_state().CanDash = true;
  //   m_CharacterComp->get_global_state().DashCooldown = m_CharacterComp->get_dash_cooldown();
  // }

  m_CharacterComp->set_velocity(playerVel);

  if(m_CharacterComp->get_velocity().length() < 1.0f && m_CharacterComp->is_on_floor()) {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::IDLE));
  }

  if(playerVel.y < -1.0f || !m_CharacterComp->is_on_floor()) {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::FALL));
  }
}

void SprintMovementState::_exit() 
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Jump Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void JumpMovementState::_enter()
{ 
  m_CharacterComp = m_MovementStateMachine->get_character_component();
  
  _jump();
}

void JumpMovementState::_handle_input(const Ref<InputEvent>& event) 
{
}

void JumpMovementState::_jump()
{
  Vector3 playerVel = m_CharacterComp->get_velocity();

  playerVel.y = m_CharacterComp->get_jump_height();
  
  Vector3 gravity_vec = (m_CharacterComp->get_floor_normal() + Vector3(0.0f, 1.0f, 0.0f)).normalized() * m_CharacterComp->get_jump_height();
  m_CharacterComp->set_gravity_vec(gravity_vec);

  m_CharacterComp->set_velocity(playerVel);
}

void JumpMovementState::_physics_update(double delta) 
{
  m_CharacterComp->_update_input();
  m_CharacterComp->_update_velocity();

  Vector3 playerVel = m_CharacterComp->get_velocity();

  if(playerVel.y < -1.0f || !m_CharacterComp->is_on_floor()) {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::FALL));
  }
  
  if (m_CharacterComp->is_on_floor()) {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::IDLE));
  }
}


void JumpMovementState::_exit() 
{
  m_CharacterComp->set_gravity_vec(Vector3(0.0f, 0.0f, 0.0f));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Fall Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void FallMovementState::_enter()
{ 
  m_CharacterComp = m_MovementStateMachine->get_character_component();
}

void FallMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if (!m_IsJumpPressed && Input::get_singleton()->is_action_just_pressed("jump")) 
  {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::JUMP));
    m_IsJumpPressed = true;
  }

  // if(Input::get_singleton()->is_action_just_pressed("dash") && m_CharacterComp->get_global_state().CanDash)
  // {
  //   m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::DASH));
  // }

  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_IsCrouchPressed == false)
  {
    m_IsCrouchPressed = true;
  }
}

void FallMovementState::_physics_update(double delta) 
{
  m_CharacterComp->_update_input();
  m_CharacterComp->_update_velocity();

  Vector3 playerVel = m_CharacterComp->get_velocity();
  Vector3 wishDir = m_CharacterComp->get_wish_dir().normalized();

  Vector3 gravity_vec = Vector3(0.0f, -1.0f, 0.0f) * m_CharacterComp->get_down_gravity() * delta;
  m_CharacterComp->set_gravity_vec(gravity_vec);
  
  float targetX = wishDir.x * m_CharacterComp->get_max_air_move_speed();
  float targetZ = wishDir.z * m_CharacterComp->get_max_air_move_speed();

  if (wishDir.length() > 0.0f) {
    playerVel.x =  Utils::exp_decay(playerVel.x, targetX, 15.0f, delta);
    playerVel.z =  Utils::exp_decay(playerVel.z, targetZ, 15.0f, delta);
  }

  // if(m_CharacterComp->get_global_state().DashCooldown <= 0.0f)
  // {
  //   m_CharacterComp->get_global_state().CanDash = true;
  //   m_CharacterComp->get_global_state().DashCooldown = m_CharacterComp->get_dash_cooldown();
  // }

  m_CharacterComp->set_velocity(playerVel);

  if(m_CharacterComp->is_on_floor())
  {
    if(m_IsCrouchPressed == true)
    {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::CROUCH));
    } else {
      m_IsJumpPressed = false;
      m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::IDLE));
    }
  }
}

void FallMovementState::_exit() 
{
  m_CharacterComp->set_gravity_vec(Vector3(0.0f, 0.0f, 0.0f));
  m_IsCrouchPressed = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Crouch Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CrouchMovementState::_enter()
{ 
  m_CharacterComp = m_MovementStateMachine->get_character_component();

  m_FinalPos = m_CharacterComp->get_character_head()->get_position().y - m_CharacterComp->get_crouch_translate();
}

void CrouchMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  
  if (Input::get_singleton()->is_action_just_pressed("crouch") && !m_CharacterComp->get_crouch_raycast()->is_colliding()) {
    _on_crouch_finished();
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::IDLE));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("jump") && !m_CharacterComp->get_crouch_raycast()->is_colliding()) 
  {
    _on_crouch_finished();
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::JUMP));
  }
}

void CrouchMovementState::_on_crouch_finished()
{
  m_CharacterComp->get_crouch_collision_shape()->set_disabled(true);
  m_CharacterComp->get_default_collision_shape()->set_disabled(false);

  if(m_CrouchTween != nullptr)
  {
    m_CrouchTween->kill();
  }

  m_CrouchTween = m_CharacterComp->create_tween();
  m_CrouchTween->tween_property(m_CharacterComp->get_character_head(), "position:y", 0.0f, 0.1f);
}

void CrouchMovementState::_physics_update(double delta) 
{
  m_CharacterComp->_update_input();    
  m_CharacterComp->_update_velocity();

  Vector3 playerVel = m_CharacterComp->get_velocity();
  Vector3 playerHeadPos = m_CharacterComp->get_character_head()->get_position();

  m_CharacterComp->get_crouch_collision_shape()->set_disabled(false);
  m_CharacterComp->get_default_collision_shape()->set_disabled(true);

  if(m_MovementStateMachine->get_prev_state() == static_cast<int8_t>(MovementStates::SLIDE))
  {
    float finalCrouchPos = m_FinalPos - m_CharacterComp->get_character_head()->get_position().y;
    playerHeadPos.y =  Utils::exp_decay(playerHeadPos.y, finalCrouchPos, m_CharacterComp->get_crouch_translate_speed(), (float)delta);
  } else {
    playerHeadPos.y =  Utils::exp_decay(playerHeadPos.y, m_FinalPos, m_CharacterComp->get_crouch_translate_speed(), (float)delta);
  }

  m_CharacterComp->get_character_head()->set_position(playerHeadPos);

  playerVel = m_CharacterComp->get_crouch_speed() * m_CharacterComp->get_wish_dir();
  m_CharacterComp->set_velocity(playerVel);

  if(m_MovementStateMachine->get_prev_state() == static_cast<int8_t>(MovementStates::FALL))
  {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::CROUCH));
  }

  if(playerVel.y < -1.0f || !m_CharacterComp->is_on_floor()) 
  {
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::FALL));
  }
}

void CrouchMovementState::_exit() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Slide Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void SlideMovementState::_enter()
{ 
  m_CharacterComp = m_MovementStateMachine->get_character_component();
  m_SlideVector = m_CharacterComp->get_wish_dir();

  m_OriginalHeadPosition = m_CharacterComp->get_character_head()->get_position();
  m_FinalPos = m_CharacterComp->get_character_head()->get_position().y - m_CharacterComp->get_crouch_translate();
  m_SlideTimer = m_CharacterComp->get_slide_timer();
}


void SlideMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump") && !m_CharacterComp->get_crouch_raycast()->is_colliding()) {
    _on_slide_finished();
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::JUMP));
  }

}

void SlideMovementState::_on_slide_finished()
{ 
  m_CharacterComp->get_crouch_collision_shape()->set_disabled(true);
  m_CharacterComp->get_default_collision_shape()->set_disabled(false);
  if(m_CrouchTween != nullptr)
  {
    m_CrouchTween->kill();
  }

  m_CrouchTween = m_CharacterComp->create_tween();
  m_CrouchTween->tween_property(m_CharacterComp->get_character_head(), "position:y", m_OriginalHeadPosition.y, 0.1f);
}

void SlideMovementState::_crouch_player()
{
  if(m_CrouchTween != nullptr) {
    m_CrouchTween->kill();
  }
  // Set collider states
  m_CharacterComp->get_crouch_collision_shape()->set_disabled(false);
  m_CharacterComp->get_default_collision_shape()->set_disabled(true);

  m_CrouchTween = m_CharacterComp->create_tween();
  m_CrouchTween->tween_property(m_CharacterComp->get_character_head(), "position:y", m_FinalPos, 0.1f);
}

void SlideMovementState::_physics_update(double delta) 
{
  m_CharacterComp->_update_input();    
  m_CharacterComp->_update_velocity();
  
  Vector3 playerVel = m_CharacterComp->get_velocity();
  Vector3 horizVel = Vector3(playerVel.x, 0.0f, playerVel.z);

  _crouch_player();

  m_SlideTimer -= delta;

  horizVel.x = m_SlideVector.x * m_CharacterComp->get_slide_speed() * m_SlideTimer;
  horizVel.z = m_SlideVector.z * m_CharacterComp->get_slide_speed() * m_SlideTimer;
  
  // if(m_CharacterComp->test_move(m_CharacterComp->get_transform(), Vector3(m_SlideVector.x, 0.0f, m_SlideVector.z))) {
  //   _on_slide_finished();
  //   // emit_signal("state_changed", "Idle");
  // }
  playerVel = Vector3(horizVel.x, playerVel.y, horizVel.z);
  m_CharacterComp->set_velocity(playerVel);

  if(m_SlideTimer <= 0.0f) {
    if(m_CharacterComp->get_crouch_raycast()->is_colliding())
    {
      _on_slide_finished();
      m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::CROUCH));
    } else {
      _on_slide_finished();
      m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::IDLE));
    }
  }

  if(playerVel.y < -1.0f || !m_CharacterComp->is_on_floor()) 
  {
    _on_slide_finished();
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::FALL));
  }
}

void SlideMovementState::_exit() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Dash Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void DashMovementState::_enter()
{ 
  m_CharacterComp = m_MovementStateMachine->get_character_component();
  m_DashDir = m_CharacterComp->get_wish_dir();
}

void DashMovementState::_handle_input(const Ref<InputEvent>& event) 
{
}

void DashMovementState::_physics_update(double delta) 
{
  m_CharacterComp->_update_input();    
  m_CharacterComp->_update_velocity();

  // m_CharacterComp->get_global_state().CanDash = false;
  
  Vector3 playerVel = m_CharacterComp->get_velocity();
  
  playerVel.x = m_DashDir.x * m_CharacterComp->get_dash_speed();
  playerVel.z = m_DashDir.z * m_CharacterComp->get_dash_speed();

  m_CharacterComp->set_velocity(playerVel);

  if(m_CharacterComp->get_velocity().length() > 0.0f)
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::SPRINT));
  else
    m_MovementStateMachine->_change_state(static_cast<int8_t>(MovementStates::IDLE));
  
}

void DashMovementState::_exit() 
{
}
