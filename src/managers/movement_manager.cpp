#include "movement_manager.h"

void MovementManager::_init()
{
  set_physics_process(false);
  set_process(false);

  ERR_FAIL_COND_MSG(!character_component, "[Movement Manager]: Character Component is null");
  ERR_FAIL_COND_MSG(!input_command_system, "[Movement Manager]: Input Command System is null");

  m_CharacterHead = character_component->get_character_head();
  m_CharCollider = character_component->get_default_collision_shape();
  m_CharColliderShape = m_CharCollider->get_shape();

  ERR_FAIL_COND_MSG(!m_CharacterHead, "[Movement Manager]: Character Head is null");
  
  const MoveCommand& move_cmd = input_command_system->get_move_command();
  m_FinalPos = m_CharacterHead->get_position().y - character_component->get_crouch_translate();

  // This is weird but I don't wanna move this to the player (for now)
  if(should_use_step_handler)
  {
    Player* player = Object::cast_to<Player>(get_tree()->get_first_node_in_group("player"));

    m_StairsBelowRaycast = player->get_stairs_below_raycast();
    m_StairsAheadRayCast = player->get_stairs_ahead_raycast();

    // For some reason, if I use StepHandlerComponent with a custom constructor like this, it won't let me compile. So using it as a pure C++ object
    m_StepHandlerComponent = memnew(StepHandlerComponent(
      StepHandlerData {
        character_component, m_StairsBelowRaycast, m_StairsAheadRayCast, m_MovementStateCtx, move_cmd
      }
    ));
  }

  print_line("Movement Manager Initialized");
}

void MovementManager::_bind_methods()
{
  GD_BIND_PROPERTY(MovementManager, should_use_step_handler, Variant::BOOL);
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
}

void MovementManager::_physics_update(double delta)
{
  const MoveCommand& move_cmd = input_command_system->get_move_command();

  character_component->_update_gravity(delta);

  if(move_cmd.IsOnFloor)
  {
    m_MovementStateCtx.LastFrameOnFloor = Engine::get_singleton()->get_physics_frames();
  }

  if(should_use_step_handler)
  {
    if(!m_StepHandlerComponent) return;
    if(!m_StepHandlerComponent->_snap_up_stairs_check(delta))
    {
      character_component->_update_velocity();
      m_StepHandlerComponent->_snap_down_to_stairs_check();
    }
  } else {
    character_component->_update_velocity();
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
  m_MovementStateCtx.IsSprinting = true;

  if(!move_cmd.IsOnFloor)
    return;

  Vector3 characterVel = move_cmd.CharacterVelocity;

  characterVel.x = Math::move_toward(characterVel.x, character_component->get_sprint_speed() * character_component->get_wish_dir().x, character_component->get_ground_accel() * (float)delta);
  characterVel.z = Math::move_toward(characterVel.z, character_component->get_sprint_speed() * character_component->get_wish_dir().z, character_component->get_ground_accel() * (float)delta);

  character_component->set_velocity(characterVel);
}

void MovementManager::_sprint_end()
{
  m_MovementStateCtx.IsSprinting = false;
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

  character_component->set_velocity(characterVel);
}

void MovementManager::_fall_end()
{
  m_MovementStateCtx.IsFalling = false;
  m_MovementStateCtx.IsCrouchPressed = false;
}


void MovementManager::_set_crouch_collider()
{
  Vector3 charColliderPos = m_CharCollider->get_position();

  m_MovementStateCtx.CharacterOriginalCapsuleHeight = m_CharColliderShape->get_height();
  m_MovementStateCtx.CharacterCurrentCapsuleHeight = m_MovementStateCtx.CharacterOriginalCapsuleHeight - character_component->get_crouch_translate();

  charColliderPos.y = m_MovementStateCtx.CharacterCurrentCapsuleHeight / 2.0f;
  
  m_CharColliderShape->set_height(m_MovementStateCtx.CharacterCurrentCapsuleHeight);
  m_CharCollider->set_position(charColliderPos);
}

void MovementManager::_set_default_collider()
{
  Vector3 charColliderPos = m_CharCollider->get_position();
  
  m_MovementStateCtx.CharacterCurrentCapsuleHeight = m_MovementStateCtx.CharacterOriginalCapsuleHeight;
  charColliderPos.y = m_MovementStateCtx.CharacterCurrentCapsuleHeight / 2.0f;
  
  m_CharColliderShape->set_height(m_MovementStateCtx.CharacterCurrentCapsuleHeight);
  m_CharCollider->set_position(charColliderPos);
}


void MovementManager::_on_crouch_start()
{
  _set_crouch_collider();
  m_MovementStateCtx.IsCrouching = true;
}

void MovementManager::_crouch(double delta)
{
  if(!character_component->is_on_floor())
    return;

  Vector3 characterVel = character_component->get_velocity();
  Vector3 characterHeadPos = m_MovementStateCtx.CharacterHeadPos;
  
  // Set the head position
  m_DampedSpring.CalcDampedSpringMotionParams(delta, character_component->get_crouch_ang_freq(), character_component->get_crouch_damping_ratio());
  m_DampedSpring.UpdateDampedSpringMotion(characterHeadPos, m_CrouchTranslateVel, Vector3(0.0f, m_FinalPos, 0.0f));
  m_CharacterHead->set_position(characterHeadPos);
  
  characterVel = character_component->get_crouch_speed() * character_component->get_wish_dir();
  character_component->set_velocity(characterVel);
}

void MovementManager::_on_crouch_finished()
{
  _set_default_collider();

  m_MovementStateCtx.IsCrouching = false;

  if(m_CrouchTween != nullptr)
  {
    m_CrouchTween->kill();
  }

  m_CrouchTween = character_component->create_tween();
  m_CrouchTween->tween_property(m_CharacterHead, "position:y", 0.0f, 0.1f);
}

void MovementManager::_on_slide_start()
{
  const MoveCommand& move_cmd = input_command_system->get_move_command();
  Vector3 characterHeadPos = m_MovementStateCtx.CharacterHeadPos;

  _set_crouch_collider();

  if(characterHeadPos.y >= m_FinalPos)
  {
    m_MovementStateCtx.IsSliding = true;
  }

  m_MovementStateCtx.IsSlideStarted = true;
  m_MovementStateCtx.IsSlideOver = false;
  m_MovementStateCtx.SlideTimer = character_component->get_slide_timer();
  m_MovementStateCtx.CharacterSlideVector = move_cmd.CharacterWishDir;
}

void MovementManager::_slide(double delta)
{
  Vector3 characterVel = character_component->get_velocity();
  Vector3 characterHeadPos = m_MovementStateCtx.CharacterHeadPos;
  Vector3 horizVel = Vector3(characterVel.x, 0.0f, characterVel.z);

  // Set the head position
  m_DampedSpring.CalcDampedSpringMotionParams(delta, character_component->get_crouch_ang_freq(), character_component->get_crouch_damping_ratio());
  m_DampedSpring.UpdateDampedSpringMotion(characterHeadPos, m_CrouchTranslateVel, Vector3(0.0f, m_FinalPos, 0.0f));
  m_CharacterHead->set_position(characterHeadPos);

  m_MovementStateCtx.SlideTimer -= delta;

  horizVel.x = m_MovementStateCtx.CharacterSlideVector.x * character_component->get_slide_speed() * m_MovementStateCtx.SlideTimer;
  horizVel.z = m_MovementStateCtx.CharacterSlideVector.z * character_component->get_slide_speed() * m_MovementStateCtx.SlideTimer;
  
  characterVel = Vector3(horizVel.x, characterVel.y, horizVel.z);
  character_component->set_velocity(characterVel);
}

void MovementManager::_on_slide_finished()
{ 
  _set_default_collider(); 

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