#include "player_slide_state.h"

void PlayerSlideState::_enter()
{ 
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_SlideVector = m_PlayerInst->get_wish_dir();

  m_OriginalHeadPosition = m_PlayerInst->get_player_head()->get_position();
  m_FinalPos = m_PlayerInst->get_player_head()->get_position().y - m_PlayerInst->get_crouch_translate();
  m_SlideTimer = 2.0f;
}

void PlayerSlideState::_bind_methods()
{
}

void PlayerSlideState::_handle_input(const Ref<InputEvent>& event) 
{
  if(Input::get_singleton()->is_action_just_pressed("jump") && !m_PlayerInst->get_collider_raycast()->is_colliding()) {
    _on_slide_finished();
    emit_signal("state_changed", "Jump");
  }

}

void PlayerSlideState::_on_slide_finished()
{ 
  m_PlayerInst->get_player_crouching_collider()->set_disabled(true);
  m_PlayerInst->get_player_standing_collider()->set_disabled(false);
  if(m_CrouchTween != nullptr)
  {
    m_CrouchTween->kill();
  }


  m_CrouchTween = m_PlayerInst->create_tween();
  m_CrouchTween->tween_property(m_PlayerInst->get_player_head(), "position:y", m_OriginalHeadPosition.y, 0.1f);
}

void PlayerSlideState::_crouch_player()
{
  if(m_CrouchTween != nullptr) {
    m_CrouchTween->kill();
  }
  // Set collider states
  m_PlayerInst->get_player_crouching_collider()->set_disabled(false);
  m_PlayerInst->get_player_standing_collider()->set_disabled(true);

  m_CrouchTween = m_PlayerInst->create_tween();
  m_CrouchTween->tween_property(m_PlayerInst->get_player_head(), "position:y", m_FinalPos, 0.1f);
}

void PlayerSlideState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();    
  m_PlayerInst->_update_velocity();
  
  Vector3 playerVel = m_PlayerInst->get_velocity();
  Vector3 horizVel = Vector3(playerVel.x, 0.0f, playerVel.z);

  _crouch_player();

  m_SlideTimer -= delta;

  horizVel.x = m_SlideVector.x * m_PlayerInst->get_slide_speed() * m_SlideTimer;
  horizVel.z = m_SlideVector.z * m_PlayerInst->get_slide_speed() * m_SlideTimer;
  
  // TODO: This is bad. Anything the player collides with will cancel the slide state. A normal body like a crate, box would make sense
  // if(m_PlayerInst->test_move(m_PlayerInst->get_transform(), Vector3(m_SlideVector.x, 0.0f, m_SlideVector.z))) {
  //   _on_slide_finished();
  //   emit_signal("state_changed", "Idle");
  // }
  playerVel = Vector3(horizVel.x, playerVel.y, horizVel.z);
  m_PlayerInst->set_velocity(playerVel);

  if(m_SlideTimer <= 0.0f) {
    if(m_PlayerInst->get_collider_raycast()->is_colliding())
    {
      _on_slide_finished();
      emit_signal("state_changed", "Crouch");
    } else {
      _on_slide_finished();
      emit_signal("state_changed", "Idle");
    }
  }

  if(playerVel.y < -1.0f || !m_PlayerInst->is_on_floor()) 
  {
    _on_slide_finished();
    emit_signal("state_changed", "Fall");
  }
}

void PlayerSlideState::_exit() 
{
}
