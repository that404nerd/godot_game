#include "player_falling_state.h"

void PlayerFallingState::_enter()
{ 
    auto sm = Object::cast_to<PlayerStateMachine>(get_parent());
    m_PlayerInst = sm->get_player_inst();
}

void PlayerFallingState::_bind_methods()
{

}

void PlayerFallingState::_handle_input(const Ref<InputEvent>& event) 
{

}

void PlayerFallingState::_physics_update(double delta) 
{
}


void PlayerFallingState::_exit() {}; 