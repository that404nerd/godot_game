#include "player_crouch_state.h"

void PlayerCrouchState::_enter()
{ 
  m_PlayerInst = m_PlayerStateMachine->get_player_inst();

  m_FinalPos = m_PlayerInst->get_player_head()->get_position().y - m_PlayerInst->get_crouch_translate();
}

void PlayerCrouchState::_handle_input(const Ref<InputEvent>& event) 
{
  // TODO: Make this work for objects like boxes, crates and stuff.
  // TODO2 : Switch from normal test_move check to a raycast for better results.
  if (Input::get_singleton()->is_action_just_pressed("crouch") && !m_PlayerInst->get_collider_raycast()->is_colliding()) {
    _on_crouch_finished();
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::IDLE));
  }
  
  if(Input::get_singleton()->is_action_just_pressed("jump") && !m_PlayerInst->get_collider_raycast()->is_colliding()) 
  {
    _on_crouch_finished();
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::JUMP));
  }
}

void PlayerCrouchState::_on_crouch_finished()
{
  m_PlayerInst->get_player_crouching_collider()->set_disabled(true);
  m_PlayerInst->get_player_standing_collider()->set_disabled(false);

  if(m_CrouchTween != nullptr)
  {
    m_CrouchTween->kill();
  }

  m_CrouchTween = m_PlayerInst->create_tween();
  m_CrouchTween->tween_property(m_PlayerInst->get_player_head(), "position:y", 0.0f, 0.1f);
}

void PlayerCrouchState::_physics_update(double delta) 
{
  m_PlayerInst->_update_input();    
  m_PlayerInst->_update_velocity();

  Vector3 playerVel = m_PlayerInst->get_velocity();
  Vector3 playerHeadPos = m_PlayerInst->get_player_head()->get_position();

  m_PlayerInst->get_player_crouching_collider()->set_disabled(false);
  m_PlayerInst->get_player_standing_collider()->set_disabled(true);

  if(m_PlayerStateMachine->get_prev_state() == static_cast<int8_t>(PlayerStates::SLIDE))
  {
    float finalCrouchPos = m_FinalPos - m_PlayerInst->get_player_head()->get_position().y;
    playerHeadPos.y = Utils::exp_decay(playerHeadPos.y, finalCrouchPos, m_PlayerInst->get_crouch_translate_speed(), (float)delta);
  } else {
    playerHeadPos.y = Utils::exp_decay(playerHeadPos.y, m_FinalPos, m_PlayerInst->get_crouch_translate_speed(), (float)delta);
  }

  m_PlayerInst->get_player_head()->set_position(playerHeadPos);

  playerVel = m_PlayerInst->get_crouch_speed() * m_PlayerInst->get_wish_dir();
  m_PlayerInst->set_velocity(playerVel);

  if(m_PlayerStateMachine->get_prev_state() == static_cast<int8_t>(PlayerStates::FALL))
  {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::SLIDE));
  }

  if(playerVel.y < -1.0f || !m_PlayerInst->is_on_floor()) 
  {
    m_PlayerStateMachine->_change_state(static_cast<int8_t>(PlayerStates::FALL));
  }
}

void PlayerCrouchState::_exit() 
{
}
