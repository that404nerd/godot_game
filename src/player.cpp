#include "player.h"

Player::Player()
{
}

void Player::_bind_methods()
{
}

void Player::_ready()
{
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{
}

void Player::_process(double delta)
{
  _update(delta);
}

void Player::_physics_process(double delta)
{
  _physics_update(delta);
}