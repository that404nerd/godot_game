#include "character_component.h"
#include "../movement_manager.h"
#include "../movement_state_machine.h"
#include "./weapon_component.h"
#include "../weapon_manager.h"
#include "../weapon_state_machine.h"
#include "./weapon_effects_components.h"

void CharacterComponent::_bind_methods()
{
  ADD_GROUP("Component Nodes", "");
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, movement_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, movement_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, weapon_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, weapon_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, weapon_effects_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, input_command_system, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ADD_GROUP("Character Nodes", "");
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, character_head, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, crouch_raycast, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, stairs_ahead_raycast, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, stairs_below_raycast, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, default_collision_shape, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CharacterComponent, crouch_collision_shape, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  
  ADD_GROUP("Speed Settings", "");
  GD_BIND_PROPERTY(CharacterComponent, crouch_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, sprint_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, slide_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, dash_speed, Variant::FLOAT);

  ADD_GROUP("Jump Settings", "");
  GD_BIND_PROPERTY(CharacterComponent, jump_height, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, down_gravity, Variant::FLOAT);

  ADD_GROUP("Crouch Settings", "");
  GD_BIND_PROPERTY(CharacterComponent, crouch_translate, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, crouch_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, crouch_damping_ratio, Variant::FLOAT);

  ADD_GROUP("Slide Settings", "");
  GD_BIND_PROPERTY(CharacterComponent, slide_timer, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, slide_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, slide_damping_ratio, Variant::FLOAT);

  ADD_GROUP("Air Strafe Settings", "");
  GD_BIND_PROPERTY(CharacterComponent, max_air_move_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, mouse_sensitivity, Variant::FLOAT);

  ADD_GROUP("Acceleration Settings", "");
  GD_BIND_PROPERTY(CharacterComponent, ground_accel, Variant::FLOAT);
  GD_BIND_PROPERTY(CharacterComponent, ground_decel, Variant::FLOAT);

  ADD_GROUP("Misc Settings", "");
  GD_BIND_PROPERTY(CharacterComponent, dash_cooldown, Variant::FLOAT);
}

void CharacterComponent::_update_input(double delta) 
{
  Vector3 characterVel = get_velocity();

  if(!is_on_floor())
  {
    characterVel.y -= down_gravity * delta;
  }

  m_InputDir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized();
  m_WishDir = get_global_transform().basis.xform(Vector3(m_InputDir.x, 0.0f, m_InputDir.y)).normalized();
  
  if (is_on_floor())
  {
    if (m_WishDir != Vector3(0.0f, 0.0f, 0.0f))
    {
      characterVel.x = Utils::exp_decay(characterVel.x, m_WishDir.x, 15.0f, ground_accel);
      characterVel.z = Utils::exp_decay(characterVel.z, m_WishDir.z, 15.0f, ground_accel);
    }
    else
    {
      characterVel.x = Utils::exp_decay(characterVel.x, 0.0f, 1.0f, ground_decel);
      characterVel.z = Utils::exp_decay(characterVel.z, 0.0f, 1.0f, ground_decel);
    }
  }

  set_velocity(characterVel);
}

void CharacterComponent::_process(double delta)
{
  input_command_system->_update(delta);
  movement_manager->_update(delta);
  movement_state_machine->_update(delta);
  weapon_manager->_update(delta);
  weapon_state_machine->_update(delta);
  weapon_effects_component->_update(delta);
}

void CharacterComponent::_physics_process(double delta)
{
  movement_manager->_physics_update(delta);
  movement_state_machine->_physics_update(delta);
  weapon_manager->_physics_update(delta);
  weapon_state_machine->_physics_update(delta);
}

void CharacterComponent::_update_velocity()
{
  move_and_slide();
}