#include "player_jump_state.h"

void PlayerJumpState::_enter()
{ 
    auto sm = Object::cast_to<PlayerStateMachine>(get_parent());
    m_PlayerInst = sm->get_player_inst();

    Vector3 playerVel = m_PlayerInst->get_velocity();
    playerVel.y = Globals::JUMP_VELOCITY;
    m_PlayerInst->set_velocity(playerVel);

    // m_CurrentJumps = MAX_JUMPS;

}

void PlayerJumpState::_bind_methods()
{
}

void PlayerJumpState::_handle_input(const Ref<InputEvent>& event) 
{
    
}

void PlayerJumpState::_physics_update(double delta) 
{
    Vector3 playerVel = m_PlayerInst->get_velocity();

    playerVel.y -= 10.0f * delta;
    m_PlayerInst->_update_input();
    m_PlayerInst->_update_velocity(); 

    // if (Input::get_singleton()->is_action_just_pressed("jump") && m_CurrentJumps > 0) {
    //     playerVel.y = Globals::JUMP_VELOCITY;
    //     m_CurrentJumps--;
    // }

    m_PlayerInst->set_velocity(playerVel);
    
    if(playerVel.y < 2.0f && !m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", "fall");
    }
    
    if (m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", "idle");
    }
}


void PlayerJumpState::_exit() {
}
