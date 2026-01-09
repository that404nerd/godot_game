#include "player_dash_state.h"

void PlayerDashState::_enter()
{ 
    m_StateMachine = Object::cast_to<PlayerStateMachine>(get_parent());
    m_PlayerInst = m_StateMachine->get_player_inst();
}

void PlayerDashState::_bind_methods()
{
}

void PlayerDashState::_handle_input(const Ref<InputEvent>& event) 
{
}

void PlayerDashState::_physics_update(double delta) 
{
  m_PlayerInst->_update_gravity(delta);
  m_PlayerInst->_update_input();    
  m_PlayerInst->_update_velocity();

  Vector3 playerVel = m_PlayerInst->get_velocity();

  Vector3 horizVel = Vector3(playerVel.x, 0.0f, playerVel.z);

  playerVel.x = horizVel.x;
  playerVel.z = horizVel.z;

  m_PlayerInst->set_velocity(playerVel);

  if(m_PlayerInst->is_on_floor())
      emit_signal("state_changed", Globals::SetCurrentState(Globals::StateNames::SPRINT));

}


void PlayerDashState::_exit() {}
