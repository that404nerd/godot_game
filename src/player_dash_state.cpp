#include "player_dash_state.h"

void PlayerDashState::_enter()
{ 
    auto sm = Object::cast_to<PlayerStateMachine>(get_parent());
    m_PlayerInst = sm->get_player_inst();
}

void PlayerDashState::_bind_methods()
{
}

void PlayerDashState::_handle_input(const Ref<InputEvent>& event) 
{
}

void PlayerDashState::_physics_update(double delta) 
{
    m_PlayerInst->_update_gravity(delta);
    m_PlayerInst->_update_input();    

    Vector3 playerVel = m_PlayerInst->get_velocity();
    
    Vector3 horizVel = Vector3(playerVel.x, 0.0f, playerVel.z);
    horizVel *= 3.0f; // Dash multiplier

    playerVel.x = horizVel.x;
    playerVel.z = horizVel.z;

    m_PlayerInst->set_velocity(playerVel);
    m_PlayerInst->_update_velocity();

    if(m_PlayerInst->is_on_floor())
        emit_signal("state_changed", "sprint");

}


void PlayerDashState::_exit() {}