#include "player_state.h"
#include "player_state_machine.h"
#include "player.h"

PlayerState::PlayerState() : m_PlayerInst(nullptr) {}

void PlayerState::_bind_methods() 
{
    ADD_SIGNAL(MethodInfo("state_changed", PropertyInfo(Variant::STRING, "stateName")));
}
