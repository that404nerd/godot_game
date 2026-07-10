#include "state_machine.h"

void StateMachine::_ready()
{
  _init_data();
  if(m_InitialState) {
    m_InitialState->_enter();
    m_CurrentState = m_InitialState;
  }
}

void StateMachine::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("get_current_state"), &StateMachine::get_current_state);
  ClassDB::bind_method(D_METHOD("get_prev_state"), &StateMachine::get_prev_state);
}

void StateMachine::_unhandled_input(const Ref<InputEvent>& event)
{
  _handle_state_machine_input(event);
  if(m_CurrentState) {
    m_CurrentState->_handle_input(event);
  }
}

void StateMachine::_physics_process(double delta)
{
  if(m_CurrentState) {
    m_CurrentState->_physics_update(delta);
  }
}

void StateMachine::_process(double delta)
{
  _handle_state_machine_process(delta);

  if(m_CurrentState)
    m_CurrentState->_update(delta);  
}

void StateMachine::_change_state(int stateID)
{
  State* newState = m_States.at(stateID).get();

  if(newState == nullptr) {
    print_error("New state not found!");
    return;
  }

  if(m_CurrentState) {
    m_CurrentState->_exit();
  }

  newState->_enter();
  m_PrevState = m_CurrentState;
  m_CurrentState = newState;
}

int StateMachine::get_current_state()
{
  if(m_CurrentState == nullptr)
  {
    print_error("Current state is null!");
    return -1;
  }

  return m_CurrentState->get_current_state();
}

int StateMachine::get_prev_state()
{
  uint prev_state_id;
  if(m_PrevState == nullptr) {
    print_error("Prev State doesn't exist!");
    return -1;
  } else {
    prev_state_id = m_PrevState->get_current_state();
  }

  return prev_state_id;
}