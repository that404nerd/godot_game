#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

#include "state.h"
#include "../components/character_component.h"
#include "../singletons/event_bus.h"

using namespace godot;

class MovementManager;
class MovementStateMachine;
struct MovementStateData;
enum class MovementStates;

struct MovementStateCtx
{
  Vector3 CharacterVelocity = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 CharacterWishDir = Vector3(0.0f, 0.0f, 0.0f);
  Vector2 CharacterInputDir = Vector2(0.0f, 0.0f);
  Vector3 CharacterSlideVector = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 CharacterHeadPos = Vector3(0.0f, 0.0f, 0.0f);

  float CharacterSprintSpeed = 0.0f;
  float SlideTimer = 0.0f;
  float DashCooldown { 0.0f };

  bool IsSlideStarted = false;
  bool IsSlideEnded = false;

  bool IsOnFloor = false;
  bool IsJumpPressed = false;
  bool IsCrouchPressed = false;
  bool IsCrouchRayCastColliding = false;
  bool CanDash = true;
};

class BaseMovementState : public State {
public:
  BaseMovementState(MovementStates movementState, const MovementStateData& movementStateData);

protected:
  MovementStateMachine* m_MovementStateMachine { nullptr };
  MovementManager* m_MovementManager { nullptr };

  const MovementStateCtx& m_MovementStateCtx;
};

///////////////////////////// Movement States Declaration //////////////////////////////////
class IdleMovementState : public BaseMovementState {
public:
  IdleMovementState(const MovementStateData& movementStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;
};

class SprintMovementState : public BaseMovementState {
public:
  SprintMovementState(const MovementStateData& movementStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;
};

class JumpMovementState : public BaseMovementState {
public:
  JumpMovementState(const MovementStateData& movementStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  CharacterComponent* m_CharacterComponent { nullptr };
};

class FallMovementState : public BaseMovementState {
public:
  FallMovementState(const MovementStateData& movementStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

};

class CrouchMovementState : public BaseMovementState {
public:
  CrouchMovementState(const MovementStateData& movementStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

};

class SlideMovementState : public BaseMovementState {
public:
  SlideMovementState(const MovementStateData& movementStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

};

class DashMovementState : public BaseMovementState {
public:
  DashMovementState(const MovementStateData& movementStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;
};