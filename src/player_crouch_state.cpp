#include "player_crouch_state.h"

void PlayerCrouchState::_enter()
{ 
    m_StateMachineInst = Object::cast_to<PlayerStateMachine>(get_parent());
    m_PlayerInst = m_StateMachineInst->get_player_inst();

    m_OriginalHeadPosition = m_PlayerInst->get_player_head()->get_position();
    m_FinalPos = m_PlayerInst->get_player_head()->get_position().y - Globals::CROUCH_TRANSLATE;
}

void PlayerCrouchState::_bind_methods()
{
}

void PlayerCrouchState::_handle_input(const Ref<InputEvent>& event) 
{
    if (Input::get_singleton()->is_action_just_pressed("crouch")) {
        _on_crouch_finished();
        emit_signal("state_changed", "idle");
    }
    
    if(Input::get_singleton()->is_action_just_pressed("jump"))
    {
        _on_crouch_finished();
        emit_signal("state_changed", "jump");
    }
}

void PlayerCrouchState::_on_crouch_finished()
{
    m_PlayerInst->get_player_crouching_collider()->set_disabled(true);
    m_PlayerInst->get_player_standing_collider()->set_disabled(false);
    
    if(m_CrouchTween != nullptr) {
        m_CrouchTween->kill();
    }

    m_CrouchTween = m_PlayerInst->create_tween();

    m_CrouchTween->tween_property(m_PlayerInst->get_player_head(), "position:y", m_OriginalHeadPosition.y, 0.2f);
}

void PlayerCrouchState::_physics_update(double delta) 
{
    m_PlayerInst->_update_gravity(delta);
    m_PlayerInst->_update_input();    
    m_PlayerInst->_update_velocity();

    Vector3 playerVel = m_PlayerInst->get_velocity();
    
    if(m_CrouchTween == nullptr || !m_CrouchTween->is_valid()) {
        m_CrouchTween = m_PlayerInst->create_tween();
        m_CrouchTween->tween_property(m_PlayerInst->get_player_head(), "position:y", m_FinalPos, 0.2f);
    }
    
    // Set collider states
    m_PlayerInst->get_player_crouching_collider()->set_disabled(false);
    m_PlayerInst->get_player_standing_collider()->set_disabled(true);
    
    playerVel = Globals::CrouchSpeed * m_PlayerInst->get_wish_dir();
    m_PlayerInst->set_velocity(playerVel);
    
    if(m_StateMachineInst->get_prev_state() == StringName("Sprint"))
    {
        emit_signal("state_changed", "slide");
    }

}


void PlayerCrouchState::_exit() 
{
}