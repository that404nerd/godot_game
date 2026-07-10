#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>

#include "components/character_component.h"
#include "states/movement_states.h"

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
  void _idle(double delta);
  void _sprint(double delta);
  void _jump();
  void _fall(double delta);


  void _on_crouch_finished();
  void _crouch(double delta);

  void _on_slide_finished();
  void _slide(double delta);
  void _dash(double delta);

protected:
  static void _bind_methods();

public:
  void init_slide_timer() { m_MovementStateCtx.SlideTimer = character_component->get_slide_timer(); }

  void set_slide_vector(Vector3 slide_vector) { m_MovementStateCtx.CharacterSlideVector = slide_vector; }
  void set_gravity_vec(Vector3 gravity_vec) { character_component->set_gravity_vec(gravity_vec); }
  void set_jump_pressed(bool status) { m_MovementStateCtx.IsJumpPressed = status; }
  void set_crouch_pressed(bool status) { m_MovementStateCtx.IsCrouchPressed = status; }

  MovementStateCtx& get_movement_state_ctx() { return m_MovementStateCtx; }
 
private:
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(MovementStateMachine*, movement_state_machine, nullptr);

private:
  Ref<Tween> m_CrouchTween { nullptr };
  Node3D* m_CharacterHead { nullptr };

private:
  MovementStateCtx m_MovementStateCtx;

  float m_FinalPos { 0.0f };
  Vector3 m_DashDir { Vector3(0.0f, 0.0f, 0.0f) };
  Vector3 m_OriginalHeadPosition { Vector3(0.0f, 0.0f, 0.0f) };
};