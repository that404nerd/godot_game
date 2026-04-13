#include "state_machine.h"

StateMachine::StateMachine()
{
  m_GlobalStateHandlerInst = memnew(GlobalStateHandler);
}

void StateMachine::_bind_methods() 
{
  GD_BIND_CUSTOM_PROPERTY(StateMachine, initial_state, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  ClassDB::bind_method(D_METHOD("_change_state", "stateName"), &StateMachine::_change_state);
}

void StateMachine::_ready()
{
  for (auto& child : get_children()) {
    Node *node = Object::cast_to<Node>(child);
    
    StringName key = node->get_name();
    State *playerState = Object::cast_to<State>(node);
    
    if (playerState) {
      playerState->connect("state_changed", Callable(this, "_change_state"));
      m_States[key] = playerState;
    }
  }
  
  if(initial_state) {
    print_line("Initial state is: ", initial_state->get_name());
    initial_state->_enter();
    m_CurrentState = initial_state;
  }

}

void StateMachine::_input(const Ref<InputEvent>& event)
{
  if(m_CurrentState) {
    m_CurrentState->_handle_input(event);
  }
}

void StateMachine::_physics_process(double delta)
{
  if(m_CurrentState) {
    m_CurrentState->_physics_update(delta);
    m_GlobalStateHandlerInst->_physics_process(delta);

    // print_line("Current state: ", get_current_state());
  }

}

void StateMachine::_change_state(const String& stateName)
{
  State* new_state = m_States.get(stateName);

  if(!new_state) {
    print_error("New state not found!");
  }

  if(m_CurrentState) {
    m_CurrentState->_exit();
  }

  new_state->_enter();
  m_PrevState = m_CurrentState;
  m_CurrentState = new_state;
}

StringName StateMachine::get_current_state()
{
  StringName current_state;
  if(m_CurrentState)
    current_state = m_CurrentState->get_name();
  else
    print_error("State not found!");

  return current_state;
}

StringName StateMachine::get_prev_state()
{
  StringName prev_state_name;
  if(m_PrevState == nullptr) {
    CRASH_NOW_MSG("Prev State doesn't exist!");
  } else {
    prev_state_name = m_PrevState->get_name();
  }

  return prev_state_name;
}

StateMachine::~StateMachine() {}
