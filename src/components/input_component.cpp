#include "input_component.h"

InputComponent::InputComponent()
  : m_CmdData(get_command_data())
{

}

void InputComponent::_ready()
{
}

void InputComponent::_bind_methods()
{

}

void InputComponent::_unhandled_input(const Ref<InputEvent>& event)
{
  m_CmdData.InputDir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized();

  if(Input::get_singleton()->is_action_just_pressed("jump"))  m_CmdData.WantsToJump = true;
  if(Input::get_singleton()->is_action_just_pressed("crouch")) m_CmdData.WantsToCrouch = true;

  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon")) m_CmdData.WantsToShootWeapon = true;
  if(Input::get_singleton()->is_action_just_released("shoot_weapon")) m_CmdData.WantsToReleaseShoot = true;
  if(Input::get_singleton()->is_action_just_pressed("reload_weapon")) m_CmdData.WantsToReloadWeapon = true;

  
  for(int i = 0; i < get_weapon_list_size(); i++)
  {
    String inputAction = "weapon_" + String::num(i + 1, 0); // INFO: Need to match the set input action in the editor
    if(Input::get_singleton()->is_action_just_pressed(inputAction))
    {
      m_CmdData.WeaponIdx = i;
      m_CmdData.WantsToSwitchWeapon = true;
    }
  }
}

void InputComponent::_process(double delta)
{
  if(m_CmdData.InputDir != Vector2(0.0f, 0.0f))
  {
    m_CmdData.WantsToSprint = true;
  } else {
    m_CmdData.WantsToSprint = false;
  }

  if(Input::get_singleton()->is_action_pressed("shoot_weapon"))
  {
    m_HoldCounter += delta;
    if(m_HoldCounter >= 0.1f)
    {
      m_CmdData.WantsToHoldShoot = true;
    }
  }

  if(m_CmdData.WantsToReleaseShoot) m_CmdData.WantsToHoldShoot = false;

  m_CmdData.WantsToShootWeapon = false;
  m_CmdData.WantsToReleaseShoot = false;

  m_CmdData.WantsToSwitchWeapon = false;
  m_CmdData.WantsToReloadWeapon = false;
  m_CmdData.WantsToJump = false;
  m_CmdData.WantsToCrouch = false;
}