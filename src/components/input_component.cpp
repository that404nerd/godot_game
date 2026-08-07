#include "input_component.h"

InputComponent::InputComponent()
{

}

void InputComponent::_init()
{
  set_physics_process(false);
  set_process(false);
  print_error("[color=GREEN]Input Command System Initialized");
}

void InputComponent::_bind_methods()
{

}

void InputComponent::_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouseEvent = event;

  set_input_dir(Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized());

  if(Input::get_singleton()->is_action_just_pressed("jump"))  set_wants_to_jump(true);
  if(Input::get_singleton()->is_action_just_pressed("crouch")) set_wants_to_crouch(true);

  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon")) set_wants_to_shoot_weapon(true);
  if(Input::get_singleton()->is_action_just_released("shoot_weapon")) set_wants_to_release_shoot(true);
  if(Input::get_singleton()->is_action_just_pressed("reload_weapon")) set_wants_to_reload_weapon(true);

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
      set_wants_to_switch_weapon(true);
    }
  }
}

void InputComponent::_update(double delta)
{
  if(get_input_dir() != Vector2(0.0f, 0.0f))
  {
    set_wants_to_sprint(true);
  } else {
    set_wants_to_sprint(false);
  }

  if(Input::get_singleton()->is_action_pressed("shoot_weapon"))
  {
    m_HoldCounter += delta;
    if(m_HoldCounter >= get_max_hold_time())
    {
      set_wants_to_hold_shoot(true);
    }
  }

  if(wants_to_release_shoot()) set_wants_to_hold_shoot(false);

  set_wants_to_shoot_weapon(false);
  set_wants_to_release_shoot(false);
  set_wants_to_switch_weapon(false);
  set_wants_to_reload_weapon(false);
  set_wants_to_jump(false);
  set_wants_to_crouch(false);
}