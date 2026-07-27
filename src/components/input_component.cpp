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
  if(Input::get_singleton()->is_action_just_pressed("jump"))  m_CmdData.WantsToJump = true;
  if(Input::get_singleton()->is_action_just_pressed("crouch"))
    m_CmdData.WantsToCrouch = !m_CmdData.WantsToCrouch;

  if(Input::get_singleton()->is_action_just_released("jump")) m_CmdData.WantsToJump = false;

  print_line(m_CmdData.WantsToCrouch);
}