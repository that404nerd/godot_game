#include "state_machine.h"

void StateMachine::_enter()
{
  if(m_InitialState) {
    m_InitialState->_enter();
    m_CurrentState = m_InitialState;
  }
}

void StateMachine::_unhandled_input(const Ref<InputEvent>& event)
{
  if(m_CurrentState) {
    m_CurrentState->_handle_input(event);
  }
}

void StateMachine::_physics_update(double delta)
{
  if(m_CurrentState) {
    m_CurrentState->_physics_update(delta);
    // print_line("Current state: ", get_current_state());
  }
}

void StateMachine::_update(double delta)
{
  m_CurrentState->_update(delta);  
}

void StateMachine::_change_state(uint8_t stateID)
{
  State* newState = m_States.at(stateID).get();

  if(!newState) {
    print_error("New state not found!");
  }

  if(m_CurrentState) {
    m_CurrentState->_exit();
  }

  newState->_enter();
  m_PrevState = m_CurrentState;
  m_CurrentState = newState;
}

uint8_t StateMachine::get_current_state()
{
  if(m_CurrentState == nullptr)
  {
    print_error("Current state is null!");
    return -1;
  }

  return m_CurrentState->get_current_state();
}

uint8_t StateMachine::get_prev_state()
{
  uint8_t prev_state_id;
  if(m_PrevState == nullptr) {
    print_error("Prev State doesn't exist!");
    return -1;
  } else {
    prev_state_id = m_PrevState->get_current_state();
  }

  return prev_state_id;
}