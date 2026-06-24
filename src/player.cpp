#include "player.h"

Player::Player()
{
  // get_global_state().DashCooldown = dash_cooldown;
}

void Player::_bind_methods()
{


}

void Player::_ready()
{
  m_GlobalStateHandler = memnew(GlobalStateHandler(this));

  m_GlobalStateHandler->_enter();
  
  m_CameraControllerNode = get_node<Node3D>(NodePath("CameraController"));
  m_PlayerCamera = get_node<Camera3D>(NodePath("%PlayerCamera"));
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{
}

void Player::_process(double delta)
{
}

void Player::_physics_process(double delta) 
{
  m_GlobalStateHandler->_physics_update(delta);
}

Player::~Player()
{
  memfree(m_GlobalStateHandler);
}
