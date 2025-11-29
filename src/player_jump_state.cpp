#include "player_jump_state.h"
#include "player_state_machine.h"

void PlayerJumpState::_enter()
{ 
    set_player_inst(Object::cast_to<Player>(get_parent()->get_parent()));
    m_PlayerInst = get_player_inst();

    m_PlayerVel = m_PlayerInst->get_velocity();

    apply_jump();
}

void PlayerJumpState::_bind_methods()
{
    
}

void PlayerJumpState::_handle_input(const Ref<InputEvent>& event) 
{
    
}

void PlayerJumpState::apply_jump()
{
    m_PlayerInst->get_jump_buffer_timer()->start();

    if(m_PlayerInst->is_on_floor()) {
        m_PlayerVel.y = Globals::JUMP_VELOCITY;
        m_PlayerInst->set_velocity(m_PlayerVel);
        m_PlayerInst->get_jump_buffer_timer()->stop();
    }
    
}

void PlayerJumpState::_physics_update(double delta) 
{
    // Apply gravity 
    if(!m_PlayerInst->is_on_floor()) {
        m_PlayerVel.y -= 9.8f * delta;
        m_PlayerInst->set_velocity(m_PlayerVel);
    }
    
    float currentSpeed = m_PlayerVel.dot(m_PlayerInst->get_wish_dir());
    float addSpeed = Globals::MaxAirMoveSpeed - currentSpeed; // how much speed we can add to the player before exceeding max move speed in air
    
    if (addSpeed > 0.0f) {
        float accel = Globals::MaxAirAccel * addSpeed;
        m_PlayerVel += m_PlayerInst->get_wish_dir() * (accel * delta); // v = u + a * t
    }
    
    m_PlayerInst->set_velocity(m_PlayerVel);

    if(m_PlayerInst->get_jump_buffer_timer()->get_time_left() > 0.0f) {
        apply_jump();
    }

    if(m_PlayerVel.y <= 0.0f && !m_PlayerInst->is_on_floor()) {
    }

    if(m_PlayerInst->is_on_floor() && m_PlayerInst->get_jump_buffer_timer()->get_time_left() <= 0.0f) {
        emit_signal("state_changed", "idle");
    }
}


void PlayerJumpState::_exit() {}