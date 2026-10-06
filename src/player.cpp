#include "player.h"

Player::Player()
{
}

void Player::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(Player, WeaponEffectsComponent, weapon_effects_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void Player::_ready()
{
  CharacterComponent::_ready();
  m_InputCmdSystem = get_input_command_system();

  if(weapon_effects_component)
    weapon_effects_component->_init();
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{
}

void Player::_process(double delta)
{
  m_InputDir = m_InputCmdSystem->get_input_dir();
  set_wish_dir(get_global_transform().basis.xform(Vector3(m_InputDir.x, 0.0f, m_InputDir.y)).normalized());

  CharacterComponent::_process(delta);

  if(weapon_effects_component)
    weapon_effects_component->_update(delta);
}

void Player::_physics_process(double delta)
{
  CharacterComponent::_physics_process(delta);
}