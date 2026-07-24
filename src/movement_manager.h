#pragma once

#include <cmath>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/physics_test_motion_parameters3d.hpp>
#include <godot_cpp/classes/physics_test_motion_result3d.hpp>

#include "components/character_component.h"
#include "godot_cpp/classes/ray_cast3d.hpp"
#include "states/movement_states.h"
#include "utils/damped_spring.h"

using namespace godot;

struct MovementStateCtx;
class MovementStateMachine;
enum class MovementStates;

class MovementManager : public Node {
  GDCLASS(MovementManager, Node);

public:
  void _ready() override;
  void _process(double delta) override;
  void _physics_process(double delta) override;

public:
  void _idle();
  void _idle_exit();

  void _sprint(double delta);
  void _sprint_end();

  void _jump();
  void _jump_end();

  void _fall(double delta);
  void _fall_end();

  void _on_crouch_finished();
  void _crouch(double delta);

  void _slide_crouch_effect(double delta);
  void _on_slide_start();
  void _slide(double delta);
  void _on_slide_finished();

  void _dash(double delta);

protected:
  static void _bind_methods();

public:

  bool is_surface_too_steep(Vector3 normal) { return normal.angle_to(Vector3(0.0f, 1.0f, 0.0f)) > character_component->get_floor_max_angle(); };
  bool _run_body_test_motion(Transform3D from, Vector3 motion, Ref<PhysicsTestMotionResult3D> result);

  void _snap_down_to_stairs_check();
  bool _snap_up_stairs_check(double delta);

  bool IsSlideStarted() { return m_MovementStateCtx.IsSlideStarted; }
  bool IsSliding() { return m_MovementStateCtx.IsSliding; }
  bool IsSlideOver() { return m_MovementStateCtx.IsSlideOver; }

  bool IsCrouching() { return m_MovementStateCtx.IsCrouching; }
  bool IsSprinting() { return m_MovementStateCtx.IsSprinting; }

  bool IsJumping() { return m_MovementStateCtx.IsJumping; }
  bool IsJumpEnded() { return m_MovementStateCtx.IsJumpEnded; }
  bool IsFalling() { return m_MovementStateCtx.IsFalling; }

  Vector3& GetCharacterVel() { return m_MovementStateCtx.CharacterVelocity; }

  void set_jump_pressed(bool status) { m_MovementStateCtx.IsJumpPressed = status; }
  void set_crouch_pressed(bool status) { m_MovementStateCtx.IsCrouchPressed = status; }

  MovementStateCtx& get_movement_state_ctx() { return m_MovementStateCtx; }
 
private:
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);

private:
  RayCast3D *m_StairsBelowRaycast { nullptr }, *m_StairsAheadRayCast { nullptr };
  Ref<Tween> m_CrouchTween { nullptr };
  Node3D* m_CharacterHead { nullptr };

private:
  MovementStateCtx m_MovementStateCtx;
  DampedSpring m_DampedSpring {};
  Vector3 m_CrouchTranslateVel {};

  const float MAX_STEP_HEIGHT { 0.5f };
  bool _snapped_to_stairs_last_frame { false };
  int _last_frame_on_floor = -INFINITY;

  float m_FinalPos { 0.0f };
  Vector3 m_DashDir { Vector3(0.0f, 0.0f, 0.0f) };
  Vector3 m_OriginalHeadPosition { Vector3(0.0f, 0.0f, 0.0f) };
};