#include "player_state_machine.h"

PlayerStateMachine::PlayerStateMachine()
{
  GameManager::get_singleton()->set_state_machine_inst(this);

  m_GlobalStateHandlerInst = memnew(GlobalStateHandler);
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
}

void PlayerStateMachine::_bind_methods() 
{
  GD_BIND_CUSTOM_PROPERTY(PlayerStateMachine, initial_state, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  ClassDB::bind_method(D_METHOD("_change_state", "stateName"), &PlayerStateMachine::_change_state);
}

void PlayerStateMachine::_ready()
{
  for (auto& child : get_children()) {
    Node *node = Object::cast_to<Node>(child);
    
    StringName key = node->get_name();
    PlayerState *playerState = Object::cast_to<PlayerState>(node);
    
    if (playerState) {
      playerState->connect("state_changed", Callable(this, "_change_state"));
      m_States[key] = playerState;
    }
  }

  
  if(initial_state) {
    initial_state->_enter();
    m_CurrentState = initial_state;
  }
}

void PlayerStateMachine::_unhandled_input(const Ref<InputEvent>& event)
{
  if(m_CurrentState) {
    m_CurrentState->_handle_input(event);
  }
}

void PlayerStateMachine::_physics_process(double delta)
{
  if(m_CurrentState) {
    m_CurrentState->_physics_update(delta);
    m_GlobalStateHandlerInst->_physics_process(delta);

    // print_line("Current state: ", get_current_state());
  }
}

void PlayerStateMachine::_change_state(const String& stateName)
{
  PlayerState* new_state = m_States.get(stateName);

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

StringName PlayerStateMachine::get_current_state()
{
  StringName current_state;
  if(m_CurrentState)
      current_state = m_CurrentState->get_name();
  else
      print_error("State not found!");

  return current_state;
}

StringName PlayerStateMachine::get_prev_state()
{
  StringName prev_state_name;
  if(m_PrevState == nullptr) {
      print_error("Prev state doesn't exist!");
  } else {
      prev_state_name = m_PrevState->get_name();
  }

  return prev_state_name;
}

PlayerStateMachine::~PlayerStateMachine() {}
