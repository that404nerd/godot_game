#include "movement_states.h"
#include "../movement_state_machine.h"
#include "../movement_manager.h"

BaseMovementState::BaseMovementState(MovementStates movementState, const MovementStateData& movementStateData)
    : State(static_cast<int>(movementState)), m_MovementStateMachine(movementStateData.MovementStateMachineInst),
      m_MovementManager(movementStateData.MovementManagerInst),
      m_MovementStateCtx(movementStateData.MovementManagerInst->get_movement_state_ctx()) {};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Idle Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

IdleMovementState::IdleMovementState(const MovementStateData& movementStateData) :
    BaseMovementState(MovementStates::IDLE, movementStateData) {};

void IdleMovementState::_enter()
{ 
}

void IdleMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump") && m_MovementStateCtx.IsOnFloor) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_MovementStateCtx.IsOnFloor)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
  }

}

void IdleMovementState::_physics_update(double delta) 
{
  m_MovementManager->_idle(delta);
  
  if(m_MovementStateCtx.CharacterInputDir != Vector2(0.0f, 0.0f) && m_MovementStateCtx.IsOnFloor) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::SPRINT));
  }

  if(m_MovementStateCtx.CharacterVelocity.y < -1.0f || !m_MovementStateCtx.IsOnFloor) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::FALL));
  }
}


void IdleMovementState::_exit() {};


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Sprint Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

SprintMovementState::SprintMovementState(const MovementStateData& movementStateData) :
  BaseMovementState(MovementStates::SPRINT, movementStateData) {};

void SprintMovementState::_enter()
{ 
}

void SprintMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump")) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_MovementStateCtx.IsOnFloor)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
  }
  
  if(m_MovementStateCtx.CharacterVelocity.length() > (m_MovementStateCtx.CharacterSprintSpeed * 0.9f) && Input::get_singleton()->is_action_just_pressed("crouch"))
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::SLIDE));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("dash") && m_MovementStateCtx.CanDash == true)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::DASH));
  }

}

void SprintMovementState::_physics_update(double delta) 
{
  m_MovementManager->_sprint(delta);
  
  Vector3 characterVel = m_MovementStateCtx.CharacterVelocity;

  if(characterVel.length() < 1.0f && m_MovementStateCtx.IsOnFloor) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
  }

  if(characterVel.y < -1.0f || !m_MovementStateCtx.IsOnFloor) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::FALL));
  }
}

void SprintMovementState::_exit() 
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Jump Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

JumpMovementState::JumpMovementState(const MovementStateData& movementStateData) :
    BaseMovementState(MovementStates::JUMP, movementStateData), m_CharacterComponent(movementStateData.MovementManagerInst->get_character_component()) {};

void JumpMovementState::_enter()
{ 
  m_MovementManager->_jump();
}

void JumpMovementState::_handle_input(const Ref<InputEvent>& event) 
{
}

void JumpMovementState::_physics_update(double delta) 
{
  Vector3 characterVel = m_MovementStateCtx.CharacterVelocity;

  if(characterVel.y < -1.0f || !m_MovementStateCtx.IsOnFloor) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::FALL));
  }
  
  if (m_MovementStateCtx.IsOnFloor) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
  }
}


void JumpMovementState::_exit() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Fall Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

FallMovementState::FallMovementState(const MovementStateData& movementStateData) :
  BaseMovementState(MovementStates::FALL, movementStateData) {};

void FallMovementState::_enter()
{ 
}

void FallMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if (Input::get_singleton()->is_action_just_pressed("jump") && m_MovementStateCtx.IsJumpPressed == false) 
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
    m_MovementManager->set_jump_pressed(true);
  }

  if(Input::get_singleton()->is_action_just_pressed("dash") && m_MovementStateCtx.CanDash == true)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::DASH));
  }

  if(Input::get_singleton()->is_action_just_pressed("crouch") && m_MovementStateCtx.IsCrouchPressed == false)
  {
    m_MovementManager->set_crouch_pressed(true);
  }
}

void FallMovementState::_physics_update(double delta) 
{
  m_MovementManager->_fall(delta);

  if(m_MovementStateCtx.IsOnFloor)
  {
    if(m_MovementStateCtx.IsCrouchPressed == true)
    {
      m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
    } else {
      m_MovementManager->set_jump_pressed(false);
      m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
    }
  }
}

void FallMovementState::_exit() 
{
  m_MovementManager->set_gravity_vec(Vector3(0.0f, 0.0f, 0.0f));
  m_MovementManager->set_crouch_pressed(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Crouch Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CrouchMovementState::CrouchMovementState(const MovementStateData& movementStateData) :
  BaseMovementState(MovementStates::CROUCH, movementStateData) {};

void CrouchMovementState::_enter()
{ 
}

void CrouchMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if (Input::get_singleton()->is_action_just_pressed("crouch") && !m_MovementStateCtx.IsCrouchRayCastColliding) {
    m_MovementManager->_on_crouch_finished();
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("jump") && !m_MovementStateCtx.IsCrouchRayCastColliding)
  {
    m_MovementManager->_on_crouch_finished();
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
  }
}


void CrouchMovementState::_physics_update(double delta) 
{
  m_MovementManager->_crouch(delta);

  Vector3 characterVel = m_MovementStateCtx.CharacterVelocity;

  if(m_MovementStateMachine->get_prev_state() == static_cast<int>(MovementStates::FALL))
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
  }

  if(characterVel.y < -1.0f || !m_MovementStateCtx.IsOnFloor) 
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::FALL));
  }
}

void CrouchMovementState::_exit() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Slide Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

SlideMovementState::SlideMovementState(const MovementStateData& movementStateData) :
  BaseMovementState(MovementStates::SLIDE, movementStateData) {};

void SlideMovementState::_enter()
{ 
  m_MovementManager->init_slide_timer();
  m_MovementManager->set_slide_vector(m_MovementStateCtx.CharacterWishDir);
}

void SlideMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump") && !m_MovementStateCtx.IsCrouchRayCastColliding) {
    m_MovementManager->_on_slide_finished();
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
  }
}

void SlideMovementState::_physics_update(double delta) 
{
  Vector3 characterVel = m_MovementStateCtx.CharacterVelocity;

  m_MovementManager->_slide(delta);

  if(m_MovementStateCtx.SlideTimer <= 0.0f) {
    if(m_MovementStateCtx.IsCrouchRayCastColliding)
    {
      m_MovementManager->_on_slide_finished();
      m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
    } else {
      m_MovementManager->_on_slide_finished();
      m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
    }
  }

  if(characterVel.y < -1.0f || !m_MovementStateCtx.IsOnFloor) 
  {
    m_MovementManager->_on_slide_finished();
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::FALL));
  }
}

void SlideMovementState::_exit() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Dash Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

DashMovementState::DashMovementState(const MovementStateData& movementStateData) :
    BaseMovementState(MovementStates::DASH, movementStateData) {};

void DashMovementState::_enter()
{ 
}

void DashMovementState::_handle_input(const Ref<InputEvent>& event) 
{
}

void DashMovementState::_physics_update(double delta) 
{
  m_MovementManager->_dash(delta);

  if(m_MovementStateCtx.CharacterVelocity.length() > 0.0f)
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::SPRINT));
  else
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
  
}

void DashMovementState::_exit() 
{
}