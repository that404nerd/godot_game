#include "input_component.h"

InputComponent::InputComponent()
{
  set_physics_process(false);
  set_process(false);
  print_line("Input Command System Initialized");
}

void InputComponent::_bind_methods()
{

}

void InputComponent::_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouseEvent = event;

  set_input_dir(Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized());

  if(Input::get_singleton()->is_action_just_pressed("jump"))  command(MovementInputCommands::JUMP);
  if(Input::get_singleton()->is_action_just_pressed("crouch")) command(MovementInputCommands::CROUCH);

  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon")) command(WeaponInputCommands::SHOOT);
  if(Input::get_singleton()->is_action_just_released("shoot_weapon")) command(WeaponInputCommands::RELEASE_SHOOT);
  if(Input::get_singleton()->is_action_just_pressed("reload_weapon")) command(WeaponInputCommands::RELOAD);

  if(event->is_class("InputEventMouseMotion")) {
    float swayIntensity = 0.005f; 

    Vector2 relative = mouseEvent->get_relative(); 

    set_mouse_vel(Vector2(-relative.x * swayIntensity, -relative.y * swayIntensity));
  }
  
  for(int i = 0; i < get_weapon_list_size(); i++)
  {
    String inputAction = "weapon_" + String::num(i + 1, 0); // INFO: Need to match the set input action in the editor
    if(Input::get_singleton()->is_action_just_pressed(inputAction))
    {
      set_weapon_idx(i);
      command(WeaponInputCommands::SWITCH_WEAPON);
    }
  }
}

void InputComponent::_update(double delta)
{
  if(get_input_dir() != Vector2(0.0f, 0.0f) || get_character_wish_dir() != Vector3(0.0f, 0.0f, 0.0f))
  {
    command(MovementInputCommands::SPRINT);
  }

  if(get_input_dir() == Vector2(0.0f, 0.0f) && get_character_wish_dir() == Vector3(0.0f, 0.0f, 0.0f))
  {
    command(MovementInputCommands::IDLE);
  }

  if(Input::get_singleton()->is_action_pressed("shoot_weapon"))
  {
    m_HoldCounter += delta;
    if(m_HoldCounter >= get_max_hold_time())
    {
      command(WeaponInputCommands::HOLD_SHOOT);
    }
  }

  if(wants_to_release_shoot()) command(WeaponInputCommands::RELEASE_SHOOT);
}