#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

#include "../components/character_component.h"
#include "../movement_state_machine.h"

using namespace godot;

struct MovementStateCtx
{
  bool IsSprinting { false }, IsCrouching { false }, IsOnFloor { false };
  bool CanDash = true;
  float DashCooldown { 0.0f };
};

class BaseMovementState : public State {
public:
  BaseMovementState(MovementStates movementState, MovementStateMachine* movementStateMachine)
    : State(static_cast<int>(movementState)), m_MovementStateMachine(movementStateMachine) {};

protected:
  CharacterComponent* m_CharacterComp { nullptr };
  MovementStateMachine* m_MovementStateMachine { nullptr };
  // const MovementStateCtx& m_MovementStateCtx;
};

///////////////////////////// Movement States Declaration //////////////////////////////////
class IdleMovementState : public BaseMovementState {
public:
  IdleMovementState(MovementStateMachine* movementStateMachine) :
    BaseMovementState(MovementStates::IDLE, movementStateMachine) {};

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;
};

class SprintMovementState : public BaseMovementState {
public:
  SprintMovementState(MovementStateMachine* movementStateMachine) :
    BaseMovementState(MovementStates::SPRINT, movementStateMachine) {};

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;
};

class JumpMovementState : public BaseMovementState {
public:
  JumpMovementState(MovementStateMachine* movementStateMachine) :
    BaseMovementState(MovementStates::JUMP, movementStateMachine) {};

  void _jump();

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

};

class FallMovementState : public BaseMovementState {
public:
  FallMovementState(MovementStateMachine* movementStateMachine) :
    BaseMovementState(MovementStates::FALL, movementStateMachine) {};

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  bool m_IsCrouchPressed { false };
  bool m_IsJumpPressed { false };

};

class CrouchMovementState : public BaseMovementState {
public:
  CrouchMovementState(MovementStateMachine* movementStateMachine) :
    BaseMovementState(MovementStates::CROUCH, movementStateMachine) {};

  void _on_crouch_finished();

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  float m_FinalPos { 0.0f };
  Ref<Tween> m_CrouchTween { nullptr };
};

class SlideMovementState : public BaseMovementState {
public:
  SlideMovementState(MovementStateMachine* movementStateMachine) :
    BaseMovementState(MovementStates::SLIDE, movementStateMachine) {};

  void _crouch_player();
  void _on_slide_finished();

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  float m_FinalPos { 0.0f }, m_SlideTimer { 0.0f };
  Vector3 m_OriginalHeadPosition { Vector3(0.0f, 0.0f, 0.0f) };
  Vector3 m_SlideVector { Vector3(0.0f, 0.0f, 0.0f) };

  Ref<Tween> m_CrouchTween { nullptr };
};

class DashMovementState : public BaseMovementState {
public:
  DashMovementState(MovementStateMachine* movementStateMachine) :
    BaseMovementState(MovementStates::DASH, movementStateMachine) {};

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  Vector3 m_DashDir { Vector3(0.0f, 0.0f, 0.0f )};
};