#include "movement_manager.h"
#include "movement_state_machine.h"
#include "states/movement_states.h"

void MovementManager::_ready()
{
  m_CharacterHead = character_component->get_character_head();
  m_FinalPos = m_CharacterHead->get_position().y - character_component->get_crouch_translate();
}

void MovementManager::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(MovementManager, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(MovementManager, movement_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void MovementManager::_process(double delta)
{
  m_MovementStateCtx.CharacterVelocity = character_component->get_velocity();
  m_MovementStateCtx.CharacterWishDir = character_component->get_wish_dir();
  m_MovementStateCtx.CharacterInputDir = character_component->get_input_dir();
  m_MovementStateCtx.IsOnFloor = character_component->is_on_floor();
  m_MovementStateCtx.CharacterSprintSpeed = character_component->get_sprint_speed();
  m_MovementStateCtx.CharacterHeadPos = m_CharacterHead->get_position();

  m_DashDir = character_component->get_wish_dir();

  if(m_MovementStateCtx.CanDash == false)
  {
    m_MovementStateCtx.DashCooldown -= delta * 1.2f;
    if(m_MovementStateCtx.DashCooldown < 0.0f)
      return;
  }
}

void MovementManager::_physics_process(double delta)
{
  character_component->_update_input();
  character_component->_update_velocity();
}

///////////////////////////////////////////////////////////////////////
/////////////////// Movement States Implementation ///////////////////////
///////////////////////////////////////////////////////////////////////

void MovementManager::_idle(double delta)
{

}

void MovementManager::_sprint(double delta)
{
  Vector3 characterVel = character_component->get_velocity();

  if(m_MovementStateCtx.DashCooldown <= 0.0f)
  {
    m_MovementStateCtx.CanDash = true;
    m_MovementStateCtx.DashCooldown = character_component->get_dash_cooldown();
  }

  characterVel = Utils::exp_decay(characterVel, character_component->get_sprint_speed() * character_component->get_wish_dir(), 15.0f, (float)delta);
  character_component->set_velocity(characterVel);
}

void MovementManager::_jump()
{
  Vector3 characterVel = character_component->get_velocity();

  characterVel.y = character_component->get_jump_height();
  
  Vector3 gravity_vec = (character_component->get_floor_normal() + Vector3(0.0f, 1.0f, 0.0f)).normalized() * character_component->get_jump_height();

  character_component->set_gravity_vec(gravity_vec);
  character_component->set_velocity(characterVel);
}

void MovementManager::_fall(double delta)
{
  Vector3 characterVel = character_component->get_velocity();
  Vector3 wishDir = character_component->get_wish_dir().normalized();

  Vector3 gravity_vec = Vector3(0.0f, -1.0f, 0.0f) * character_component->get_down_gravity() * delta;
  character_component->set_gravity_vec(gravity_vec);
  
  float targetX = wishDir.x * character_component->get_max_air_move_speed();
  float targetZ = wishDir.z * character_component->get_max_air_move_speed();

  if (wishDir.length() > 0.0f) {
    characterVel.x = Utils::exp_decay(characterVel.x, targetX, 15.0f, delta);
    characterVel.z = Utils::exp_decay(characterVel.z, targetZ, 15.0f, delta);
  }

  if(m_MovementStateCtx.DashCooldown <= 0.0f)
  {
    m_MovementStateCtx.CanDash = true;
    m_MovementStateCtx.DashCooldown = character_component->get_dash_cooldown();
  }

  character_component->set_velocity(characterVel);
}


void MovementManager::_on_crouch_finished()
{
  character_component->get_crouch_collision_shape()->set_disabled(true);
  character_component->get_default_collision_shape()->set_disabled(false);

  if(m_CrouchTween != nullptr)
  {
    m_CrouchTween->kill();
  }

  m_CrouchTween = character_component->create_tween();
  m_CrouchTween->tween_property(m_CharacterHead, "position:y", 0.0f, 0.1f);
}

void MovementManager::_crouch(double delta)
{
  Vector3 characterVel = character_component->get_velocity();
  Vector3 characterHeadPos = m_MovementStateCtx.CharacterHeadPos;

  character_component->get_crouch_collision_shape()->set_disabled(false);
  character_component->get_default_collision_shape()->set_disabled(true);

  m_DampedSpring.CalcDampedSpringMotionParams(delta, character_component->get_crouch_ang_freq(), character_component->get_crouch_damping_ratio());
  m_DampedSpring.UpdateDampedSpringMotion(characterHeadPos, m_CrouchTranslateVel, Vector3(0.0f, m_FinalPos, 0.0f));

  m_CharacterHead->set_position(characterHeadPos);

  characterVel = character_component->get_crouch_speed() * character_component->get_wish_dir();
  character_component->set_velocity(characterVel);
}

void MovementManager::_slide_crouch_effect(double delta)
{
  Vector3 characterVel = character_component->get_velocity();
  Vector3 characterHeadPos = m_MovementStateCtx.CharacterHeadPos;

  character_component->get_crouch_collision_shape()->set_disabled(false);
  character_component->get_default_collision_shape()->set_disabled(true);

  m_DampedSpring.CalcDampedSpringMotionParams(delta, character_component->get_slide_ang_freq(), character_component->get_slide_damping_ratio());
  m_DampedSpring.UpdateDampedSpringMotion(characterHeadPos, m_CrouchTranslateVel, Vector3(0.0f, m_FinalPos, 0.0f));

  m_CharacterHead->set_position(characterHeadPos);

  if(characterHeadPos.y >= m_FinalPos)
  {
    m_MovementStateCtx.IsSlideStarted = true;
    m_MovementStateCtx.IsSlideEnded = false;
  }

  characterVel = character_component->get_crouch_speed() * character_component->get_wish_dir();
  character_component->set_velocity(characterVel);
}

void MovementManager::_on_slide_start()
{
  m_MovementStateCtx.SlideTimer = character_component->get_slide_timer();
  m_MovementStateCtx.CharacterSlideVector = m_MovementStateCtx.CharacterWishDir;

}

void MovementManager::_slide(double delta)
{
  Vector3 characterVel = character_component->get_velocity();
  Vector3 horizVel = Vector3(characterVel.x, 0.0f, characterVel.z);

  _slide_crouch_effect(delta);

  m_MovementStateCtx.SlideTimer -= delta;

  horizVel.x = m_MovementStateCtx.CharacterSlideVector.x * character_component->get_slide_speed() * m_MovementStateCtx.SlideTimer;
  horizVel.z = m_MovementStateCtx.CharacterSlideVector.z * character_component->get_slide_speed() * m_MovementStateCtx.SlideTimer;
  
  // if(character_component->test_move(m_CharacterComp->get_transform(), Vector3(m_SlideVector.x, 0.0f, m_SlideVector.z))) {
  //   _on_slide_finished();
  //   // emit_signal("state_changed", "Idle");
  // }
  characterVel = Vector3(horizVel.x, characterVel.y, horizVel.z);
  character_component->set_velocity(characterVel);
}

void MovementManager::_on_slide_finished()
{ 
  character_component->get_crouch_collision_shape()->set_disabled(true);
  character_component->get_default_collision_shape()->set_disabled(false);

  if(m_CrouchTween != nullptr)
  {
    m_CrouchTween->kill();
  }

  m_CrouchTween = character_component->create_tween();
  m_CrouchTween->tween_property(m_CharacterHead, "position:y", m_OriginalHeadPosition.y, 0.1f);

  EventBus::get_singleton()->emit_signal("slide_end");

  m_MovementStateCtx.IsSlideStarted = false;
  m_MovementStateCtx.IsSlideEnded = true;
}

void MovementManager::_dash(double delta)
{
  m_MovementStateCtx.CanDash = false;
  Vector3 characterVel = character_component->get_velocity();
  
  characterVel.x = m_DashDir.x * character_component->get_dash_speed();
  characterVel.z = m_DashDir.z * character_component->get_dash_speed();

  character_component->set_velocity(characterVel);
}