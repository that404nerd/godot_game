#include "player_sprint_state.h"
#include "player_state_machine.h"

void PlayerSprintState::_enter()
{ 
    set_player_inst(Object::cast_to<Player>(get_parent()->get_parent()));
    m_PlayerInst = get_player_inst();

    m_PlayerVel = m_PlayerInst->get_velocity();
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
}

void PlayerSprintState::_physics_update(double delta) 
{
    float currentSpeedInWishDir = m_PlayerInst->get_velocity().dot(m_PlayerInst->get_wish_dir());
    float addSpeed = m_PlayerInst->get_move_speed() - currentSpeedInWishDir;
    
    if(addSpeed > 0.0f) {
        float accel = Globals::GroundAccel * m_PlayerInst->get_move_speed() * delta;
        accel = Math::min(accel, addSpeed);
        m_PlayerVel += accel * m_PlayerInst->get_wish_dir();
    } 

    // Friciton code
    float control = Math::max(m_PlayerVel.length(), Globals::GroundDecel); // Dont let speed to drop to zero instead to ground decl when stopping
    float drop = control * Globals::GroundFriction * delta; // how much velocity should be dropped due to friction
    float newSpeed = Math::max(m_PlayerVel.length() - drop, 0.0f); // New speed has to be subtracted from the current velocity due to friction
    
    if(m_PlayerVel.length() > 0.0f) {
        newSpeed /= m_PlayerVel.length();
    }

    m_PlayerVel *= newSpeed;
    
    m_PlayerInst->set_velocity(m_PlayerVel);

    if(m_PlayerInst->get_input_dir() == Vector2(0.0f, 0.0f) && m_PlayerVel.length() == 0) {
        emit_signal("state_changed", "idle");
    }
}


void PlayerSprintState::_exit() {}