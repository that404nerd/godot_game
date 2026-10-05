#include "movement_states.h"
#include "../state_machines/movement_state_machine.h"
#include "../managers/movement_manager.h"

BaseMovementState::BaseMovementState(MovementStates movementState, const MovementStateData& movementStateData)
    : State(static_cast<int>(movementState)), m_MovementStateMachine(movementStateData.MovementStateMachineInst),
      m_MovementManager(movementStateData.MovementManagerInst), m_InputCmdSystem(m_MovementManager->get_input_command_system()),
      m_MoveCmd(m_InputCmdSystem->get_move_command()), m_MovementStateCtx(m_MovementManager->get_movement_state_ctx())
{
  if(!m_MovementManager || !m_MovementStateMachine)
  {
    print_error("[Movement State]: Movement Manager or Movement State Machine is null");
    return;
  }

  if(!m_InputCmdSystem)
  {
    print_error("[Movement State]: Input Command System is null");
    return;
  }
}

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
  if(m_MoveCmd.WantsToJump) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
  }

  if(m_MoveCmd.WantsToCrouch)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
  }

}

void IdleMovementState::_physics_update(double delta) 
{
  m_MovementManager->_idle(delta);

  Vector3 char_vel = m_MoveCmd.CharacterVelocity;
  
  if(m_MoveCmd.WantsToSprint) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::SPRINT));
  }
  
  if(m_MoveCmd.WantsToWalk)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::WALK));
  }
}


void IdleMovementState::_exit() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Idle Movement State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

WalkMovementState::WalkMovementState(const MovementStateData& movementStateData) :
    BaseMovementState(MovementStates::WALK, movementStateData) {};

void WalkMovementState::_enter()
{ 
}

void WalkMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if(m_MoveCmd.WantsToJump) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
  }
    
  if(m_MoveCmd.WantsToCrouch)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
  }

}

void WalkMovementState::_physics_update(double delta) 
{
  m_MovementManager->_walk(delta);

  if(m_MoveCmd.WantsToIdle)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
  }

  if(m_MoveCmd.WantsToSprint) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::SPRINT));
  }
}


void WalkMovementState::_exit() 
{
}


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
  if(m_MoveCmd.WantsToJump) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
  }
  
  if(m_MoveCmd.WantsToCrouch)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
  }
  
  if(m_MoveCmd.CharacterVelocity.length() > (m_MovementStateCtx.CharacterSprintSpeed * 0.85f) && m_MoveCmd.WantsToCrouch)
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::SLIDE));
  }
  
}

void SprintMovementState::_physics_update(double delta) 
{
  m_MovementManager->_sprint(delta);
  
  Vector3 characterVel = m_MoveCmd.CharacterVelocity;

  if(m_MoveCmd.WantsToIdle) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
  }

  if(characterVel.y < m_MovementStateCtx.FallThreshold || !m_MoveCmd.IsOnFloor) {
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
    BaseMovementState(MovementStates::JUMP, movementStateData) {};

void JumpMovementState::_enter()
{ 
  m_MovementManager->_jump();
}

void JumpMovementState::_handle_input(const Ref<InputEvent>& event) 
{
}

void JumpMovementState::_physics_update(double delta) 
{
  Vector3 characterVel = m_MoveCmd.CharacterVelocity;

  if(characterVel.y < m_MovementStateCtx.FallThreshold) {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::FALL));
  }
}


void JumpMovementState::_exit() 
{
  m_MovementManager->_jump_end();
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

}

void FallMovementState::_physics_update(double delta) 
{
  m_MovementManager->_fall(delta);

  if(m_MoveCmd.IsOnFloor)
  {
    if(m_MovementStateCtx.IsCrouchPressed)
    {
      m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
    } else {
      m_MovementManager->_fall_end();
      m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
    }
  }
}

void FallMovementState::_exit() 
{
  m_MovementManager->_fall_end();
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
  if (m_MoveCmd.WantsToCrouch && !m_MovementStateCtx.IsCrouchRayCastColliding) {
    m_MovementManager->_on_crouch_finished();
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
  }
  
  if(m_MoveCmd.WantsToJump && !m_MovementStateCtx.IsCrouchRayCastColliding)
  {
    m_MovementManager->_on_crouch_finished();
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
  }
}


void CrouchMovementState::_physics_update(double delta) 
{
  m_MovementManager->_crouch(delta);

  Vector3 characterVel = m_MoveCmd.CharacterVelocity;

  if(m_MovementStateMachine->get_prev_state() == static_cast<int>(MovementStates::FALL))
  {
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::CROUCH));
  }

  if(characterVel.y < m_MovementStateCtx.FallThreshold || !m_MoveCmd.IsOnFloor) 
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
  m_MovementManager->_on_slide_start(); 
}

void SlideMovementState::_handle_input(const Ref<InputEvent>& event) 
{
  if(m_MoveCmd.WantsToJump && !m_MovementStateCtx.IsCrouchRayCastColliding) {
    m_MovementManager->_on_slide_finished();
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::JUMP));
  }
}

void SlideMovementState::_physics_update(double delta) 
{
  Vector3 characterVel = m_MoveCmd.CharacterVelocity;

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

  if(characterVel.y < m_MovementStateCtx.FallThreshold || !m_MoveCmd.IsOnFloor) 
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

  if(m_MoveCmd.CharacterVelocity.length() > 0.0f)
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::SPRINT));
  else
    m_MovementStateMachine->_change_state(static_cast<int>(MovementStates::IDLE));
  
}

void DashMovementState::_exit() 
{
}