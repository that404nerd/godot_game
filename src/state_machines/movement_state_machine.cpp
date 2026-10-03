#include "movement_state_machine.h"

void MovementStateMachine::_init_data()
{
  m_MovementStateData = { movement_manager, this };

  m_States[static_cast<int>(MovementStates::IDLE)] = std::make_unique<IdleMovementState>(m_MovementStateData);
  m_States[static_cast<int>(MovementStates::WALK)] = std::make_unique<WalkMovementState>(m_MovementStateData);
  m_States[static_cast<int>(MovementStates::SPRINT)] = std::make_unique<SprintMovementState>(m_MovementStateData);
  m_States[static_cast<int>(MovementStates::JUMP)] = std::make_unique<JumpMovementState>(m_MovementStateData);
  m_States[static_cast<int>(MovementStates::FALL)] = std::make_unique<FallMovementState>(m_MovementStateData);
  m_States[static_cast<int>(MovementStates::CROUCH)] = std::make_unique<CrouchMovementState>(m_MovementStateData);
  m_States[static_cast<int>(MovementStates::SLIDE)] = std::make_unique<SlideMovementState>(m_MovementStateData);
  m_States[static_cast<int>(MovementStates::DASH)] = std::make_unique<DashMovementState>(m_MovementStateData);

  m_InitialState = m_States.at(static_cast<int>(MovementStates::IDLE)).get();
  print_line("Movement State Machine Initialized");
}

StringName MovementStateMachine::get_current_state_name()
{
  auto none_name = magic_enum::enum_name(MovementStates::NONE);
  StringName noneString = std::string(none_name).c_str();
  if(m_CurrentState == nullptr)
  {
    print_error("Current state is null!");
    return noneString;
  }

  auto state = magic_enum::enum_cast<MovementStates>(m_CurrentState->get_current_state());
  auto stateName = magic_enum::enum_name(state.value());
  StringName finalStateName = std::string(stateName).c_str();

  return finalStateName;
}

StringName MovementStateMachine::get_prev_state_name()
{
  auto none_name = magic_enum::enum_name(MovementStates::NONE);
  StringName noneString = std::string(none_name).c_str();

  if(m_PrevState == nullptr)
  {
    print_error("Prev state doesn't exist!");
    return noneString;
  }

  auto state = magic_enum::enum_cast<MovementStates>(m_PrevState->get_current_state());
  auto stateName = magic_enum::enum_name(state.value());
  StringName finalStateName = std::string(stateName).c_str();


  return finalStateName;
}

void MovementStateMachine::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(MovementStateMachine, CharacterComponent, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(MovementStateMachine, MovementManager, movement_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ClassDB::bind_method(D_METHOD("get_current_state_name"), &MovementStateMachine::get_current_state_name);
  ClassDB::bind_method(D_METHOD("get_prev_state_name"), &MovementStateMachine::get_prev_state_name);
}