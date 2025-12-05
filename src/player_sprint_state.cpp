#include "player_sprint_state.h"
#include "player.h"

void PlayerSprintState::_enter()
{ 
    set_player_inst(Object::cast_to<Player>(get_parent()->get_parent()));
    m_PlayerInst = get_player_inst();
}

void PlayerSprintState::_bind_methods()
{

}

void PlayerSprintState::_handle_input(const Ref<InputEvent>& event) 
{
    if(Input::get_singleton()->is_action_just_pressed("jump")) {
        emit_signal("state_changed", "jump");
    }

    if(Input::get_singleton()->is_action_just_pressed("crouch") && m_PlayerInst->is_on_floor())
    {
        emit_signal("state_changed", "crouch");
    }

    if(Input::get_singleton()->is_action_just_pressed("dash"))
    {
        emit_signal("state_changed", "dash");
    } 
}

void PlayerSprintState::_physics_update(double delta) 
{
    Vector3 playerVel = m_PlayerInst->get_velocity();

    m_PlayerInst->_update_gravity(delta);
    m_PlayerInst->_update_input();    
    
    float currentSpeedInWishDir = m_PlayerInst->get_velocity().dot(m_PlayerInst->get_wish_dir());
    float addSpeed = Globals::SprintSpeed - currentSpeedInWishDir;
    
    if(addSpeed > 0.0f) {
        float accel = Globals::GroundAccel * Globals::SprintSpeed * delta;
        accel = Math::min(accel, addSpeed);
        playerVel += accel * m_PlayerInst->get_wish_dir();
    } 

    // Friciton code
    float control = Math::max(playerVel.length(), Globals::GroundDecel); // Dont let speed to drop to zero instead to ground decl when stopping
    float drop = control * Globals::GroundFriction * delta; // how much velocity should be dropped due to friction
    float newSpeed = Math::max(playerVel.length() - drop, 0.0f); // New speed has to be subtracted from the current velocity due to friction
    
    if(playerVel.length() > 0.0f) {
        newSpeed /= playerVel.length();
    }
    
    playerVel *= newSpeed;
    
    m_PlayerInst->set_velocity(playerVel);
    m_PlayerInst->_update_velocity();
    
    if(m_PlayerInst->get_input_dir() == Vector2(0.0f, 0.0f) && m_PlayerInst->is_on_floor()) {
        emit_signal("state_changed", "idle");
    }
}


void PlayerSprintState::_exit() {}