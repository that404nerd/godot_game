#include "player_slide_state.h"

void PlayerSlideState::_enter()
{ 
    auto sm = Object::cast_to<PlayerStateMachine>(get_parent());
    m_PlayerInst = sm->get_player_inst();
    m_SlideVector = m_PlayerInst->get_wish_dir();

    m_PlayerCamInst = m_PlayerInst->get_player_camera();
    m_OriginalHeadPosition = m_PlayerInst->get_player_head()->get_position();
    m_SlideTimer = 2.0f;
}

void PlayerSlideState::_bind_methods()
{
}

void PlayerSlideState::_handle_input(const Ref<InputEvent>& event) 
{
    if(Input::get_singleton()->is_action_just_pressed("jump")) {
        _on_slide_finished();
        emit_signal("state_changed", "jump");
    }
}

void PlayerSlideState::_on_slide_finished()
{
    if(m_SlideTween != nullptr)
    {
        m_SlideTween->kill();
    }

     
    if(m_CrouchTween != nullptr) {
        m_CrouchTween->kill();
    }

    
    m_CrouchTween = m_PlayerInst->create_tween();
    m_SlideTween = m_PlayerCamInst->create_tween();
    
    m_CrouchTween->tween_property(m_PlayerInst->get_player_head(), "position:y", m_OriginalHeadPosition.y, 0.2f);
    m_SlideTween->tween_property(m_PlayerCamInst, "rotation:z", 0.0f, 0.2f); // Do this to smoothly reset rotation when cancelling slide
}


void PlayerSlideState::_physics_update(double delta) 
{
    m_PlayerInst->_update_gravity(delta); 
    m_PlayerInst->_update_input();    
    m_PlayerInst->_update_velocity();

    Vector3 playerVel = m_PlayerInst->get_velocity();
    Vector3 horizVel = Vector3(playerVel.x, 0.0f, playerVel.z);

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
        _on_slide_finished();
        emit_signal("state_changed", "idle");
    }
}


void PlayerSlideState::_exit() 
{
}