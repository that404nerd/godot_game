#include "player_slide_state.h"

void PlayerSlideState::_enter()
{ 
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_SlideVector = m_PlayerInst->get_wish_dir();

  m_PlayerCamInst = m_PlayerInst->get_player_camera();
  m_OriginalHeadPosition = m_PlayerInst->get_player_head()->get_position();
  m_FinalPos = m_PlayerInst->get_player_head()->get_position().y - m_PlayerInst->get_crouch_translate();
  m_SlideTimer = 2.0f;
}

void PlayerSlideState::_bind_methods()
{
}

void PlayerSlideState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump") && !m_PlayerInst->test_move(m_PlayerInst->get_transform(), Vector3(0.0f, -m_FinalPos, 0.0f))) {
    _on_slide_finished();
    emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::JUMP));
  }
}

void PlayerSlideState::_on_slide_finished()
{
  if(m_SlideTween != nullptr)
  {
    m_SlideTween->kill();
  }

  if(m_CrouchTween != nullptr)
  {
    m_CrouchTween->kill();
  }

  m_PlayerInst->get_player_crouching_collider()->set_disabled(true);
  m_PlayerInst->get_player_standing_collider()->set_disabled(false);
  
  m_CrouchTween = m_PlayerInst->create_tween();
  m_CrouchTween->tween_property(m_PlayerInst->get_player_head(), "position:y", m_OriginalHeadPosition.y, 0.1f);

  m_SlideTween = m_PlayerCamInst->create_tween();
  m_SlideTween->tween_property(m_PlayerInst->get_player_head(), "position:y", m_OriginalHeadPosition.y, 0.1f); // Use this to restore crouch position to original position when cancelling slide
  m_SlideTween->tween_property(m_PlayerCamInst, "rotation:z", 0.0f, 0.1f); // Do this to smoothly reset rotation when cancelling slide
}

void PlayerSlideState::_crouch_player()
{
  if(m_CrouchTween != nullptr) {
    m_CrouchTween->kill();
  }

  m_CrouchTween = m_PlayerInst->create_tween();
  m_CrouchTween->tween_property(m_PlayerInst->get_player_head(), "position:y", m_FinalPos, 0.1f);

  // Set collider states
  m_PlayerInst->get_player_crouching_collider()->set_disabled(false);
  m_PlayerInst->get_player_standing_collider()->set_disabled(true);

}

void PlayerSlideState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();    
  m_PlayerInst->_update_velocity();
  
  Vector3 playerVel = m_PlayerInst->get_velocity();
  Vector3 horizVel = Vector3(playerVel.x, 0.0f, playerVel.z);

  _crouch_player();

  m_SlideTimer -= delta;

  horizVel.x = m_SlideVector.x * 7.0f * m_SlideTimer;
  horizVel.z = m_SlideVector.z * 7.0f * m_SlideTimer;
  
  // TODO: This is bad. Anything the player collides with will cancel the slide state. A normal body like a crate, box would make sense
  // if(m_PlayerInst->test_move(m_PlayerInst->get_transform(), Vector3(m_SlideVector.x, 0.0f, m_SlideVector.z))) {
  //   _on_slide_finished();
  //   emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::IDLE));
  // }
  playerVel = Vector3(horizVel.x, playerVel.y, horizVel.z);
  m_PlayerInst->set_velocity(playerVel);

  if(m_SlideTween == nullptr || !m_SlideTween->is_valid()) {
      m_SlideTween = m_PlayerCamInst->create_tween();
      m_SlideTween->tween_property(m_PlayerCamInst, "rotation:z", Math::deg_to_rad(7.0f), m_SlideTimer);
  }
  
  if(m_SlideTimer <= 0.0f) {
    if(m_PlayerInst->test_move(m_PlayerInst->get_transform(), Vector3(0.0f, -m_FinalPos, 0.0f)))
    {
      _on_slide_finished();
      emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::CROUCH));
    } else {
      _on_slide_finished();
      emit_signal("state_changed", m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::IDLE));
    }
  }
}

void PlayerSlideState::_exit() 
{
}
