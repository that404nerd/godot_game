#include "input_component.h"

InputComponent::InputComponent()
  : m_InputCmdData(get_input_command_data())
{

}

void InputComponent::_ready()
{
  set_physics_process(false);
  set_process(false);
  print_line_rich("[color=GREEN]Input Command System Initialized");
}

void InputComponent::_bind_methods()
{

}

void InputComponent::_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouseEvent = event;

  m_InputCmdData.InputDir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized();

  if(Input::get_singleton()->is_action_just_pressed("jump"))  m_InputCmdData.WantsToJump = true;
  if(Input::get_singleton()->is_action_just_pressed("crouch")) m_InputCmdData.WantsToCrouch = true;

  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon")) m_InputCmdData.WantsToShootWeapon = true;
  if(Input::get_singleton()->is_action_just_released("shoot_weapon")) m_InputCmdData.WantsToReleaseShoot = true;
  if(Input::get_singleton()->is_action_just_pressed("reload_weapon")) m_InputCmdData.WantsToReloadWeapon = true;

  if(event->is_class("InputEventMouseMotion")) {
    float swayIntensity = 0.005f; 

    Vector2 relative = mouseEvent->get_relative(); 
    m_InputCmdData.MouseVel.x = -relative.x * swayIntensity;
    m_InputCmdData.MouseVel.y = -relative.y * swayIntensity;
  }
  
  for(int i = 0; i < get_weapon_list_size(); i++)
  {
    String inputAction = "weapon_" + String::num(i + 1, 0); // INFO: Need to match the set input action in the editor
    if(Input::get_singleton()->is_action_just_pressed(inputAction))
    {
      m_InputCmdData.WeaponIdx = i;
      m_InputCmdData.WantsToSwitchWeapon = true;
    }
  }
}

void InputComponent::_update(double delta)
{
  if(m_InputCmdData.InputDir != Vector2(0.0f, 0.0f))
  {
    m_InputCmdData.WantsToSprint = true;
  } else {
    m_InputCmdData.WantsToSprint = false;
  }

  if(Input::get_singleton()->is_action_pressed("shoot_weapon"))
  {
    m_HoldCounter += delta;
    if(m_HoldCounter >= m_InputCmdData.MaxHoldTime)
    {
      m_InputCmdData.WantsToHoldShoot = true;
    }
  }

  if(m_InputCmdData.WantsToReleaseShoot) m_InputCmdData.WantsToHoldShoot = false;

  m_InputCmdData.WantsToShootWeapon = false;
  m_InputCmdData.WantsToReleaseShoot = false;

  m_InputCmdData.WantsToSwitchWeapon = false;
  m_InputCmdData.WantsToReloadWeapon = false;
  m_InputCmdData.WantsToJump = false;
  m_InputCmdData.WantsToCrouch = false;
}