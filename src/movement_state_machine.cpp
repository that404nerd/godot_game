#include "movement_state_machine.h"
#include "states/movement_states.h"

void MovementStateMachine::_init_data()
{
  m_States[static_cast<int8_t>(MovementStates::IDLE)] = std::make_unique<IdleMovementState>(this);
  m_States[static_cast<int8_t>(MovementStates::SPRINT)] = std::make_unique<SprintMovementState>(this);
  m_States[static_cast<int8_t>(MovementStates::JUMP)] = std::make_unique<JumpMovementState>(this);
  m_States[static_cast<int8_t>(MovementStates::FALL)] = std::make_unique<FallMovementState>(this);
  m_States[static_cast<int8_t>(MovementStates::CROUCH)] = std::make_unique<CrouchMovementState>(this);
  m_States[static_cast<int8_t>(MovementStates::SLIDE)] = std::make_unique<SlideMovementState>(this);
  m_States[static_cast<int8_t>(MovementStates::DASH)] = std::make_unique<DashMovementState>(this);

  m_InitialState = m_States.at(static_cast<int8_t>(MovementStates::IDLE)).get();
}

void MovementStateMachine::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(MovementStateMachine, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  ClassDB::bind_method(D_METHOD("get_current_state_name"), &MovementStateMachine::get_current_state_name);
  ClassDB::bind_method(D_METHOD("get_prev_state_name"), &MovementStateMachine::get_prev_state_name);
}

void MovementStateMachine::_handle_state_machine_process(double delta)
{
  // if(!m_MovementStateCtx.CanDash)
  // {
  //   m_PlayerInst->get_global_state().DashCooldown -= delta * 1.2f;

  //   if(m_PlayerInst->get_global_state().DashCooldown < 0.0f)
  //     return;
  // }
}