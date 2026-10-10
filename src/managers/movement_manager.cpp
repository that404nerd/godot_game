#include "movement_manager.h"

void MovementManager::_init()
{
  set_physics_process(false);
  set_process(false);

  ERR_FAIL_COND_MSG(!character_component, "[Movement Manager]: Character Component is null");
  ERR_FAIL_COND_MSG(!input_command_system, "[Movement Manager]: Input Command System is null");

  const MoveCommand& move_cmd = input_command_system->get_move_command();

  m_CharacterHead = character_component->get_character_head();
  m_StairsBelowRaycast = character_component->get_stairs_below_raycast();
  m_StairsAheadRayCast = character_component->get_stairs_ahead_raycast();

  ERR_FAIL_COND_MSG(!m_CharacterHead, "[Movement Manager]: Character Head is null");

  m_FinalPos = m_CharacterHead->get_position().y - character_component->get_crouch_translate();

  m_StepHandlerComponent = memnew(StepHandlerComponent(
    StepHandlerData {
      character_component, m_StairsBelowRaycast, m_StairsAheadRayCast, m_MovementStateCtx, move_cmd
    }
  ));

  ERR_FAIL_COND_MSG(!m_StepHandlerComponent, "[Movement Manager]: Step Handler Component is null");

  print_line("Movement Manager Initialized");
}

void MovementManager::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(MovementManager, CharacterComponent, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(MovementManager, InputCommandSystem, input_command_system, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void MovementManager::_update(double delta)
{
  ERR_FAIL_COND_MSG(!character_component->get_crouch_raycast(), "[Movement Manager]: Crouch Raycast is null");

  m_MovementStateCtx.CharacterSprintSpeed = character_component->get_sprint_speed();
  m_MovementStateCtx.CharacterHeadPos = m_CharacterHead->get_position();
  m_MovementStateCtx.IsCrouchRayCastColliding = character_component->get_crouch_raycast()->is_colliding();

  input_command_system->build_move_command(character_component);

  // m_DashDir = character_component->get_wish_dir();

  // if(m_MovementStateCtx.CanDash == false)
  // {
  //   m_MovementStateCtx.DashCooldown -= delta;
  //   if(m_MovementStateCtx.DashCooldown < 0.0f)
  //     return;
  // }

}

void MovementManager::_physics_update(double delta)
{
  const MoveCommand& move_cmd = input_command_system->get_move_command();
  if(move_cmd.IsOnFloor)
  {
    m_MovementStateCtx.LastFrameOnFloor = Engine::get_singleton()->get_physics_frames();
  }

  character_component->_update_gravity(delta);

  if(m_StepHandlerComponent && !m_StepHandlerComponent->_snap_up_stairs_check(delta))
  {
    character_component->_update_velocity();
    m_StepHandlerComponent->_snap_down_to_stairs_check();
  }
}

///////////////////////////////////////////////////////////////////////
/////////////////// Movement States Implementation ///////////////////////
///////////////////////////////////////////////////////////////////////

void MovementManager::_idle(double delta)
{
  const MoveCommand& move_cmd = input_command_system->get_move_command();

  if(!move_cmd.IsOnFloor)
  {
    return;
  }

  Vector3 characterVel = move_cmd.CharacterVelocity;

  // print_line(get_owner()->get_name(), ": Going idle!");

  characterVel.x = Math::move_toward(characterVel.x, 0.0f, character_component->get_ground_decel() * (float)delta);
  characterVel.z = Math::move_toward(characterVel.z, 0.0f, character_component->get_ground_decel() * (float)delta);

  character_component->set_velocity(characterVel);
}

void MovementManager::_walk(double delta)
{
  const MoveCommand& move_cmd = input_command_system->get_move_command();

  if(!move_cmd.IsOnFloor)
    return;
  
  Vector3 characterVel = move_cmd.CharacterVelocity;

  characterVel.x = Math::move_toward(characterVel.x, character_component->get_walk_speed() * character_component->get_wish_dir().x, character_component->get_ground_accel() * (float)delta);
  characterVel.z = Math::move_toward(characterVel.z, character_component->get_walk_speed() * character_component->get_wish_dir().z, character_component->get_ground_accel() * (float)delta);

  character_component->set_velocity(characterVel);
}

void MovementManager::_sprint(double delta)
{
  const MoveCommand& move_cmd = input_command_system->get_move_command();

  if(!move_cmd.IsOnFloor)
    return;

  Vector3 characterVel = move_cmd.CharacterVelocity;

  characterVel.x = Math::move_toward(characterVel.x, character_component->get_sprint_speed() * character_component->get_wish_dir().x, character_component->get_ground_accel() * (float)delta);
  characterVel.z = Math::move_toward(characterVel.z, character_component->get_sprint_speed() * character_component->get_wish_dir().z, character_component->get_ground_accel() * (float)delta);

  character_component->set_velocity(characterVel);
}

void MovementManager::_jump()
{
  const MoveCommand& move_cmd = input_command_system->get_move_command();

  if(!move_cmd.IsOnFloor)
    return;

  Vector3 characterVel = move_cmd.CharacterVelocity;
  m_MovementStateCtx.IsJumping = true;
  m_MovementStateCtx.IsJumpEnded = false;

  characterVel.y = character_component->get_jump_height();

  character_component->set_velocity(characterVel);
}

void MovementManager::_jump_end()
{
  m_MovementStateCtx.IsJumping = false;
  m_MovementStateCtx.IsJumpEnded = true;
}

void MovementManager::_fall(double delta)
{
  const MoveCommand& move_cmd = input_command_system->get_move_command();
  m_MovementStateCtx.IsFalling = true;

  if(move_cmd.WantsToCrouch && m_MovementStateCtx.IsCrouchPressed == false)
  {
    m_MovementStateCtx.IsCrouchPressed = true;
  }

  Vector3 characterVel = character_component->get_velocity();
  Vector3 wishDir = character_component->get_wish_dir().normalized();

  float targetX = wishDir.x * character_component->get_max_air_move_speed();
  float targetZ = wishDir.z * character_component->get_max_air_move_speed();

  if (wishDir.length() > 0.0f) {
    characterVel.x = Math::move_toward(characterVel.x, targetX, (float)delta * character_component->get_air_control_factor());
    characterVel.z = Math::move_toward(characterVel.z, targetZ, (float)delta * character_component->get_air_control_factor());
  }

  // if(m_MovementStateCtx.DashCooldown <= 0.0f)
  // {
  //   m_MovementStateCtx.CanDash = true;
  //   m_MovementStateCtx.DashCooldown = character_component->get_dash_cooldown();
  // }

  character_component->set_velocity(characterVel);
}

void MovementManager::_fall_end()
{
  m_MovementStateCtx.IsFalling = false;
  m_MovementStateCtx.IsCrouchPressed = false;
}

void MovementManager::_crouch(double delta)
{
  if(!character_component->is_on_floor())
    return;

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
    m_MovementStateCtx.IsSliding = true;
  }

  characterVel = character_component->get_crouch_speed() * character_component->get_wish_dir();
  character_component->set_velocity(characterVel);
}

void MovementManager::_on_slide_start()
{
  const MoveCommand& move_cmd = input_command_system->get_move_command();

  m_MovementStateCtx.IsSlideStarted = true;
  m_MovementStateCtx.IsSlideOver = false;
  m_MovementStateCtx.SlideTimer = character_component->get_slide_timer();
  m_MovementStateCtx.CharacterSlideVector = move_cmd.CharacterWishDir;
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

  m_MovementStateCtx.IsSlideStarted = false;
  m_MovementStateCtx.IsSliding = false;
  m_MovementStateCtx.IsSlideOver = true;
}

void MovementManager::_dash(double delta)
{
  m_MovementStateCtx.CanDash = false;
  Vector3 characterVel = character_component->get_velocity();
  
  characterVel.x = m_DashDir.x * character_component->get_dash_speed();
  characterVel.z = m_DashDir.z * character_component->get_dash_speed();

  character_component->set_velocity(characterVel);
}

MovementManager::~MovementManager()
{
  if(m_StepHandlerComponent)
    memfree(m_StepHandlerComponent);
}