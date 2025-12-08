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
    m_PlayerInst->_update_input();
    m_PlayerInst->_update_velocity();

    Vector3 playerVel = m_PlayerInst->get_velocity();
    Vector3 wish = m_PlayerInst->get_wish_dir();
    
    playerVel.y -= 12.0f * delta; // Special falling velocity

    if (Input::get_singleton()->is_action_just_pressed("jump") && !m_IsJumpPressed) {
        playerVel.y = Globals::JUMP_VELOCITY;
        m_IsJumpPressed = true;
    }

    float currentSpeed = playerVel.dot(Vector3(wish.x, 0, wish.z));
    float addSpeed = Globals::MaxAirMoveSpeed - currentSpeed;
    if (addSpeed > 0.0f) {
        playerVel.x += wish.x * Globals::MaxAirAccel * addSpeed * delta;
        playerVel.z += wish.z * Globals::MaxAirAccel * addSpeed * delta;
    }
    
    m_PlayerInst->set_velocity(playerVel);
    
    if(m_PlayerInst->is_on_floor())
    {
        m_IsJumpPressed = false;
        emit_signal("state_changed", "idle");
    }
}


void PlayerFallingState::_exit() 
{
}