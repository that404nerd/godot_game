#include "player_jump_state.h"

void PlayerJumpState::_enter()
{ 
    m_PlayerInst = Object::cast_to<Player>(get_parent()->get_parent());
    m_CurrentJumps = MAX_JUMPS;

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
    Vector3 wish = m_PlayerInst->get_wish_dir();

    if (Input::get_singleton()->is_action_just_pressed("jump") && m_CurrentJumps > 0) {
        playerVel.y = Globals::JUMP_VELOCITY;
        m_CurrentJumps--;
    }

    if (!m_PlayerInst->is_on_floor()) {
        float currentSpeed = Vector3(playerVel.x, 0.0f, playerVel.z).dot(Vector3(wish.x, 0, wish.z));
        float addSpeed = Globals::MaxAirMoveSpeed - currentSpeed;
        if (addSpeed > 0.0f) {
            playerVel.x += wish.x * Globals::MaxAirAccel * addSpeed * delta;
            playerVel.z += wish.z * Globals::MaxAirAccel * addSpeed * delta;
        }
    }

    if(playerVel.y < 0.0f && !m_PlayerInst->is_on_floor()) {
    }

    m_PlayerInst->set_velocity(playerVel);
    m_PlayerInst->_update_velocity(); 

    if (m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", "idle");
    }
}


void PlayerJumpState::_exit() {
    m_CurrentJumps = MAX_JUMPS;
}
