#include "player_crouch_state.h"
#include "godot_cpp/core/math.hpp"

void PlayerCrouchState::_enter()
{ 
    m_StateMachineInst = Object::cast_to<PlayerStateMachine>(get_parent());
    m_PlayerInst = m_StateMachineInst->get_player_inst();

    m_OriginalHeadPosition = m_PlayerInst->get_player_head()->get_position();
    m_FinalPos = m_PlayerInst->get_player_head()->get_position().y - Globals::CROUCH_TRANSLATE;

    m_PlayerCamInst = m_PlayerInst->get_player_camera();

    m_SlideVector = m_PlayerInst->get_wish_dir();
    m_SlideTimer = 2.0f;
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

    if(m_SlideTween != nullptr)
    {
        m_SlideTween->kill();
    }
    
    m_CrouchTween = m_PlayerInst->create_tween();
    m_SlideTween = m_PlayerCamInst->create_tween();

    m_CrouchTween->tween_property(m_PlayerInst->get_player_head(), "position:y", m_OriginalHeadPosition.y, 0.2f);
    m_SlideTween->tween_property(m_PlayerCamInst, "rotation:z", 0.0f, 0.2f); // Do this to smoothly reset rotation when cancelling slide
}

void PlayerCrouchState::_physics_update(double delta) 
{
    m_PlayerInst->_update_gravity(delta);
    m_PlayerInst->_update_input();    
    m_PlayerInst->_update_velocity();

    
    Vector3 playerVel = m_PlayerInst->get_velocity();
    Vector3 horizVel = Vector3(playerVel.x, 0.0f, playerVel.z);
    
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
        m_SlideTimer -= delta;
        
        horizVel.x = m_SlideVector.x * 7.0f * m_SlideTimer;
        horizVel.z = m_SlideVector.z * 7.0f * m_SlideTimer;
        
        playerVel = Vector3(horizVel.x, playerVel.y, horizVel.z);
        m_PlayerInst->set_velocity(playerVel);

        if(m_SlideTween == nullptr || !m_SlideTween->is_valid()) {
            m_SlideTween = m_PlayerCamInst->create_tween();
            m_SlideTween->tween_property(m_PlayerCamInst, "rotation:z", Math::deg_to_rad(7.0f), m_SlideTimer);
        }
        
        if(m_SlideTimer <= 0.0f) {
            _on_crouch_finished();
            emit_signal("state_changed", "idle");
        }
    }

}


void PlayerCrouchState::_exit() 
{
}