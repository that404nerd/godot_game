#include "step_handler_component.h"

void StepHandlerComponent::_snap_down_to_stairs_check()
{
  bool did_snap = false;
  
  m_StepHandlerData.StairsBelowRaycast->force_raycast_update();
  
  bool floor_below = m_StepHandlerData.StairsBelowRaycast->is_colliding() && !is_surface_too_steep(m_StepHandlerData.StairsBelowRaycast->get_collision_normal());
  bool was_on_floor_last_frame = Engine::get_singleton()->get_physics_frames() - m_StepHandlerData.MovementCtx.LastFrameOnFloor == 1;

  if(!m_StepHandlerData.MovementCtx.IsOnFloor && m_StepHandlerData.MovementCtx.CharacterVelocity.y <= 0.0f && (was_on_floor_last_frame || _snapped_to_stairs_last_frame) && floor_below)
  {
    Ref<PhysicsTestMotionResult3D> body_test_result;
    body_test_result.instantiate();
    if(!body_test_result.is_valid())
    {
      print_error("body_test_result is invalid!");
      return;
    }

    if(_run_body_test_motion(m_StepHandlerData.CharacterCompInst->get_global_transform(), Vector3(0.0f, -MAX_STEP_HEIGHT, 0.0f), body_test_result))
    {
      float translate_y = body_test_result->get_travel().y;
      Vector3 charPosition = m_StepHandlerData.CharacterCompInst->get_position();
      charPosition.y += translate_y;
      m_StepHandlerData.CharacterCompInst->apply_floor_snap();
      did_snap = true;
      m_StepHandlerData.CharacterCompInst->set_position(charPosition);
    }

    _snapped_to_stairs_last_frame = did_snap;
  }
}

bool StepHandlerComponent::_snap_up_stairs_check(double delta)
{
  if(m_StepHandlerData.MovementCtx.IsOnFloor)
  {
    Vector3 expected_move_pos = m_StepHandlerData.MovementCtx.CharacterVelocity * Vector3(1.0f, 0.0f, 1.0f) * delta; // Forward vector (x, z)
    
    Transform3D step_pos_with_clearance = m_StepHandlerData.CharacterCompInst->get_global_transform().translated(expected_move_pos + Vector3(0.0f, MAX_STEP_HEIGHT * 2.0f, 0.0f));
    
    Ref<PhysicsTestMotionResult3D> down_check_result;
    down_check_result.instantiate();
    
    bool result = _run_body_test_motion(step_pos_with_clearance, Vector3(0.0f, -MAX_STEP_HEIGHT * 2.0f, 0.0f), down_check_result);
    
    if(result)
    {
      float step_height = ((step_pos_with_clearance.get_origin() + down_check_result->get_travel()) - m_StepHandlerData.CharacterCompInst->get_global_position()).y;
      
      if(step_height > MAX_STEP_HEIGHT || step_height <= 0.01f || (down_check_result->get_collision_point() - m_StepHandlerData.CharacterCompInst->get_global_position()).y > MAX_STEP_HEIGHT)
        return false;
      
      m_StepHandlerData.StairsAheadRaycast->set_global_position(down_check_result->get_collision_point() + Vector3(0.0f, MAX_STEP_HEIGHT, 0.0f) + expected_move_pos.normalized() * 0.1f);
      m_StepHandlerData.StairsAheadRaycast->force_raycast_update();
      
      if(m_StepHandlerData.StairsAheadRaycast->is_colliding() && !is_surface_too_steep(m_StepHandlerData.StairsAheadRaycast->get_collision_normal()))
      {
        Vector3 reqPos = step_pos_with_clearance.get_origin() + down_check_result->get_travel();
        
        m_StepHandlerData.CharacterCompInst->set_global_position(reqPos);
        m_StepHandlerData.CharacterCompInst->apply_floor_snap();
        
        _snapped_to_stairs_last_frame = true;
        return true;
      }
    }
  }

  return false;
  
}

bool StepHandlerComponent::_run_body_test_motion(Transform3D from, Vector3 motion, Ref<PhysicsTestMotionResult3D> result)
{
  Ref<PhysicsTestMotionParameters3D> params;

  if(!result.is_valid())
    result.instantiate();

  params.instantiate();

  if(!result.is_valid() || !params.is_valid())
  {
    print_error("PhysicsTestMotionResult3D is invalid or params are invalid!");
    return false;
  }

  params->set_from(from);
  params->set_motion(motion);

  return PhysicsServer3D::get_singleton()->body_test_motion(m_StepHandlerData.CharacterCompInst->get_rid(), params, result);
}