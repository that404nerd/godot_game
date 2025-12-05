#include "player_falling_state.h"

void PlayerFallingState::_enter()
{ 
    // set_player_inst(Object::cast_to<Player>(get_parent()->get_parent()));
    // m_PlayerInst = get_player_inst();

}

void PlayerFallingState::_bind_methods()
{
}

void PlayerFallingState::_handle_input(const Ref<InputEvent>& event) 
{
}

void PlayerFallingState::_physics_update(double delta) 
{
    print_line("Falling!");
}


void PlayerFallingState::_exit() {}