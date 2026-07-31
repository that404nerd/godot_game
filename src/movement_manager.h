#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/physics_test_motion_parameters3d.hpp>
#include <godot_cpp/classes/physics_test_motion_result3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>

#include "input_command_system.h"
#include "components/character_component.h"
#include "components/input_component.h"
#include "components/step_handler_component.h"
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
  void _update(double delta);
  void _physics_update(double delta);

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

  ~MovementManager();

protected:
  static void _bind_methods();

public:

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
  InputCommandSystem* get_input_command_system_instance() { return input_command_system; }
 
private:
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(InputCommandSystem*, input_command_system, nullptr);

private:
  RayCast3D *m_StairsBelowRaycast { nullptr }, *m_StairsAheadRayCast { nullptr };
  Ref<Tween> m_CrouchTween { nullptr };
  Node3D* m_CharacterHead { nullptr };

  StepHandlerComponent* m_StepHandlerComponent { nullptr };

private:
  MovementStateCtx m_MovementStateCtx;
  DampedSpring m_DampedSpring {};
  Vector3 m_CrouchTranslateVel {};

  float m_FinalPos { 0.0f };
  Vector3 m_DashDir { Vector3(0.0f, 0.0f, 0.0f) };
  Vector3 m_OriginalHeadPosition { Vector3(0.0f, 0.0f, 0.0f) };
};