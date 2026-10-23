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

  m_ShootCmd.WantsToPressTrigger = Input::get_singleton()->is_action_just_pressed("shoot_weapon");
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
    .WantsToSprint = (m_MoveCmd.InputDir != Vector2(0.0f, 0.0f)),
    .WantsToIdle = (m_MoveCmd.InputDir == Vector2(0.0f, 0.0f))
  };
}

void InputComponent::build_shoot_command(double delta)
{
  // Did i press the mouse?
  bool is_pressed_now = m_ShootCmd.WantsToPressTrigger;

  // Did i hold the mouse?
  bool is_held_now = m_ShootCmd.WantsToHoldTrigger;

  // Did I hold the mouse? Which is true if the hold counter reaches the max hold time.
  m_ShootCmd.WantsToHoldTrigger = is_held_now;

  // Did I press the mouse (single click)? Which is true if the mouse is pressed/held and it wasn't held before
  m_ShootCmd.WantsToPressTrigger = is_pressed_now && !m_WasHeldBefore;
  m_WasHeldBefore = is_pressed_now;

  // print_line("Is pressed: ", m_ShootCmd.WantsToPressTrigger, ", Is Held: ", m_ShootCmd.WantsToHoldTrigger);
}

void InputComponent::_update(double delta)
{
  if(Input::get_singleton()->is_action_pressed("shoot_weapon"))
  {
    m_HoldCounter += delta;
    if(m_HoldCounter >= get_max_hold_time())
      m_ShootCmd.WantsToHoldTrigger = true;
  }

  if(m_WeaponCmd.WantsToReleaseShoot)
  {
    m_ShootCmd.WantsToHoldTrigger = false;    
    m_HoldCounter = 0.0f;   
  }
}