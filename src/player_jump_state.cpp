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
    Vector3 vel = m_PlayerInst->get_velocity();

    vel.y -= 10.0f * delta;
    m_PlayerInst->_update_input();
    Vector3 wish = m_PlayerInst->get_wish_dir();

    if (Input::get_singleton()->is_action_just_pressed("jump") && m_CurrentJumps > 0 && m_PlayerInst->is_on_floor()) {
        vel.y = Globals::JUMP_VELOCITY;
        m_CurrentJumps--;
    }

    if (!m_PlayerInst->is_on_floor()) {
        float currentSpeed = Vector3(vel.x, 0, vel.z).dot(Vector3(wish.x, 0, wish.z));
        float addSpeed = Globals::MaxAirMoveSpeed - currentSpeed;
        if (addSpeed > 0.0f) {
            vel.x += wish.x * Globals::MaxAirAccel * addSpeed * delta;
            vel.z += wish.z * Globals::MaxAirAccel * addSpeed * delta;
        }
    }

    m_PlayerInst->set_velocity(vel);
    m_PlayerInst->_update_velocity(); 

    if (m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", "idle");
    }
}


void PlayerJumpState::_exit() {
    m_CurrentJumps = MAX_JUMPS;
}
