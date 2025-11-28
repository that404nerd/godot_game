#include "player_state_machine.h"

PlayerStateMachine::PlayerStateMachine() : m_CurrentState(nullptr)
{
}

void PlayerStateMachine::_bind_methods() 
{
    GD_BIND_CUSTOM_PROPERTY(PlayerStateMachine, initial_state, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE, "Node");
    ClassDB::bind_method(D_METHOD("_change_state", "stateName"), &PlayerStateMachine::_change_state);
}

void PlayerStateMachine::_ready()
{
    for (auto &child_var : get_children()) {

        Node *node = Object::cast_to<Node>(child_var);
        if (!node) continue;

        StringName key = node->get_name();
        PlayerState *playerState = Object::cast_to<PlayerState>(node);
        
        if (playerState) {
            playerState->connect("state_changed", Callable(this, "_change_state"));
            m_States[key.to_lower()] = playerState;
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
    }
}

void PlayerStateMachine::_change_state(const String& stateName)
{
    PlayerState* new_state = m_States.get(stateName.to_lower());

    // if(!new_state) {
    //     print_error("New state not found!");
    // } else {
    //     print_line("New state found!");
    // }

    if(m_CurrentState) {
        m_CurrentState->_exit();
    }

    new_state->_enter();
    m_CurrentState = new_state;
}

PlayerStateMachine::~PlayerStateMachine() {}