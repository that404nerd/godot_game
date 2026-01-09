#include "player_idle_state.h"

void PlayerIdleState::_enter()
{ 
    auto sm = Object::cast_to<PlayerStateMachine>(get_parent());
    m_PlayerInst = sm->get_player_inst();
}

void PlayerIdleState::_bind_methods()
{

}

void PlayerIdleState::_handle_input(const Ref<InputEvent>& event) 
{
    if(Input::get_singleton()->is_action_just_pressed("jump") && m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::JUMP));
    }
    
    if(Input::get_singleton()->is_action_just_pressed("crouch") && m_PlayerInst->is_on_floor())
    {
        emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::CROUCH));
    }

}

void PlayerIdleState::_physics_update(double delta) 
{
    m_PlayerInst->_update_gravity(delta);
    m_PlayerInst->_update_input();    
    m_PlayerInst->_update_velocity();
    
    if(m_PlayerInst->get_input_dir() != Vector2(0.0f, 0.0f) && m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::SPRINT));
    }

    Vector3 playerVel = m_PlayerInst->get_velocity();

    if(playerVel.y < 2.0f && !m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::FALL));
    }
}


void PlayerIdleState::_exit() {}; 
