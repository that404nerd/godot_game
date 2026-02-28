#include "player_state.h"
#include "player_state_machine.h"

PlayerState::PlayerState() {}

void PlayerState::_bind_methods() 
{
    ADD_SIGNAL(MethodInfo("state_changed", PropertyInfo(Variant::STRING, "stateName")));
}
