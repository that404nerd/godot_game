#include "input_component.h"
#include "character_component.h"

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

  m_MoveCmd.WantsToCrouch = Input::get_singleton()->is_action_just_pressed("crouch");
  m_MoveCmd.WantsToJump = Input::get_singleton()->is_action_just_pressed("jump");

  m_WeaponCmd.WantsToShootWeapon = Input::get_singleton()->is_action_just_pressed("shoot_weapon");
  m_WeaponCmd.WantsToReleaseShoot = Input::get_singleton()->is_action_just_released("shoot_weapon");
  m_WeaponCmd.WantsToReloadWeapon = Input::get_singleton()->is_action_just_pressed("reload_weapon");

  if(event->is_class("InputEventMouseMotion")) {
    float swayIntensity = 0.005f; 

    Vector2 relative = mouseEvent->get_relative(); 
    set_mouse_vel(Vector2(-relative.x * swayIntensity, -relative.y * swayIntensity));
  }
  
  for(int i = 0; i < get_weapon_res_list_size(); i++)
  {
    String inputAction = "weapon_" + String::num(i + 1, 0); // INFO: Need to match the set input action in the editor
    if(Input::get_singleton()->is_action_just_pressed(inputAction))
    {
      m_WeaponCmd.WantsToSwitchWeapon = true;
      set_weapon_idx(i);
    }

  }
}

void InputComponent::build_move_command(CharacterComponent *character_comp)
{
  m_MoveCmd = {
    .CharacterWishDir = character_comp->get_wish_dir(),
    .CharacterVelocity = character_comp->get_velocity(),
    .InputDir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized(),
    .IsOnFloor = character_comp->is_on_floor(),
    .WantsToMove = (m_MoveCmd.InputDir != Vector2(0.0f, 0.0f)),
    .WantsToSprint = (m_MoveCmd.InputDir != Vector2(0.0f, 0.0f)),
    .WantsToIdle = (m_MoveCmd.InputDir == Vector2(0.0f, 0.0f))
  };
}

void InputComponent::_update(double delta)
{
  if(Input::get_singleton()->is_action_pressed("shoot_weapon"))
  {
    m_HoldCounter += delta;
    if(m_HoldCounter >= get_max_hold_time())
    {
      m_WeaponCmd.WantsToHoldTrigger = true;
    }
  }

  if(m_WeaponCmd.WantsToReleaseShoot)
  {
    m_HoldCounter = 0.0f;   
    m_WeaponCmd.WantsToHoldTrigger = false;
  }

  m_WeaponCmd.WantsToSwitchWeapon = false;
}