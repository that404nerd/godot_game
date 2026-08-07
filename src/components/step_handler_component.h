#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/physics_test_motion_parameters3d.hpp>

#include "./character_component.h"
#include "../states/movement_states.h"

using namespace godot;

struct StepHandlerData
{
  CharacterComponent* CharacterCompInst = nullptr;
  RayCast3D* StairsBelowRaycast = nullptr;
  RayCast3D* StairsAheadRaycast = nullptr;
  const MovementStateCtx& MovementCtx;
};

class StepHandlerComponent
{
public:

  StepHandlerComponent(StepHandlerData stepHandlerData) : m_StepHandlerData(stepHandlerData) 
  {
    if(stepHandlerData.CharacterCompInst == nullptr)
    {
      print_error("[color=WHITE][StepHandler Component]: [color=RED]Character Component is null");
      return;
    }

    if(stepHandlerData.StairsAheadRaycast == nullptr)
    {
      print_error("[color=WHITE][StepHandler Component]: [color=RED]Stairs Ahead Raycast is null");
      return;
    }

    if(stepHandlerData.StairsBelowRaycast == nullptr)
    {
      print_error("[color=WHITE][StepHandler Component]: [color=RED]Stairs Below Raycast is null");
      return;
    }
  }

  bool is_surface_too_steep(Vector3 normal) { return normal.angle_to(Vector3(0.0f, 1.0f, 0.0f)) > m_StepHandlerData.CharacterCompInst->get_floor_max_angle(); };
  bool _run_body_test_motion(Transform3D from, Vector3 motion, Ref<PhysicsTestMotionResult3D> result);

  void _snap_down_to_stairs_check();
  bool _snap_up_stairs_check(double delta);

private:
  StepHandlerData m_StepHandlerData;

  const float MAX_STEP_HEIGHT { 0.25f };
  bool _snapped_to_stairs_last_frame { false };
};