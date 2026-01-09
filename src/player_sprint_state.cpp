#include "player_sprint_state.h"

void PlayerSprintState::_enter()
{ 
    auto sm = Object::cast_to<PlayerStateMachine>(get_parent());
    m_PlayerInst = sm->get_player_inst();
}

void PlayerSprintState::_bind_methods()
{
    GD_BIND_PROPERTY(PlayerSprintState, headbob_move_freq, Variant::FLOAT);
    GD_BIND_PROPERTY(PlayerSprintState, headbob_move_amt, Variant::FLOAT);
}

void PlayerSprintState::_handle_input(const Ref<InputEvent>& event) 
{
    if(Input::get_singleton()->is_action_just_pressed("jump")) {
        emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::JUMP));
    }
    
    if(Input::get_singleton()->is_action_just_pressed("crouch") && m_PlayerInst->is_on_floor())
    {
        emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::CROUCH));
    }
    
    if(Input::get_singleton()->is_action_just_pressed("dash"))
    {
        emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::DASH));
    } 
}

void PlayerSprintState::headbob_effect(double delta)
{
    Vector3 playerVel = m_PlayerInst->get_velocity();
    Vector3 playerVelHeadbob = Vector3(playerVel.x, 0.0f, playerVel.z); // Use this vector so that y-vel (gravity) doesn't affect the headbob
    m_HeadBobTime += playerVelHeadbob.length() * delta;
    
    Transform3D headbobTransform = m_PlayerInst->get_player_head()->get_transform(); // get the player's transform
    headbobTransform.origin = Vector3( // like a sine wave
        Math::sin(m_HeadBobTime * headbob_move_freq) * headbob_move_amt,
        Math::cos(m_HeadBobTime * headbob_move_freq) * headbob_move_amt,
        0.0f
    );
    m_PlayerInst->get_player_head()->set_transform(headbobTransform);
}

void PlayerSprintState::_physics_update(double delta) 
{
    m_PlayerInst->_update_gravity(delta); 
    m_PlayerInst->_update_input();    
    m_PlayerInst->_update_velocity();
    
    
    Vector3 playerVel = m_PlayerInst->get_velocity();
    float currentSpeedInWishDir = m_PlayerInst->get_velocity().dot(m_PlayerInst->get_wish_dir());
    float addSpeed = m_PlayerInst->get_sprint_speed() - currentSpeedInWishDir;
    
    if(addSpeed > 0.0f) {
        float accel = m_PlayerInst->get_ground_accel() * m_PlayerInst->get_sprint_speed() * delta;
        accel = Math::min(accel, addSpeed);
        playerVel += accel * m_PlayerInst->get_wish_dir();
    } 
    
    // Friciton code
    float control = Math::max(playerVel.length(), m_PlayerInst->get_ground_decel()); // Dont let speed to drop to zero instead to ground decl when stopping
    float drop = control * m_PlayerInst->get_ground_friction() * delta; // how much velocity should be dropped due to friction
    float newSpeed = Math::max(playerVel.length() - drop, 0.0f); // New speed has to be subtracted from the current velocity due to friction
    
    if(playerVel.length() > 0.0f) {
        newSpeed /= playerVel.length();
    }
    
    playerVel *= newSpeed;
    
    headbob_effect(delta);
    m_PlayerInst->set_velocity(playerVel);
    
    if(m_PlayerInst->get_input_dir() == Vector2(0.0f, 0.0f) && m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::IDLE));
    }

    if(playerVel.y < 1.0f && !m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::FALL));
    }

}


void PlayerSprintState::_exit() 
{
}
