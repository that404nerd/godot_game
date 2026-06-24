#pragma once

#include <godot_cpp/godot.hpp>

#include "movement_state_machine.h"
#include "player.h"

using namespace godot;

class IdleMovementState : public State {
public:
  IdleMovementState(MovementStateMachine* movementStateMachine) :
    State(static_cast<int8_t>(MovementStates::IDLE)), m_MovementStateMachine(movementStateMachine) {};

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  CharacterComponent* m_CharacterComp { nullptr };
  MovementStateMachine* m_MovementStateMachine { nullptr };
};

class SprintMovementState : public State {
public:
  SprintMovementState(MovementStateMachine* movementStateMachine) :
    State(static_cast<int8_t>(MovementStates::SPRINT)), m_MovementStateMachine(movementStateMachine) {};

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  CharacterComponent* m_CharacterComp { nullptr };
  MovementStateMachine* m_MovementStateMachine { nullptr };
};

class JumpMovementState : public State {
public:
  JumpMovementState(MovementStateMachine* movementStateMachine) :
    State(static_cast<int8_t>(MovementStates::JUMP)), m_MovementStateMachine(movementStateMachine) {};

  void _jump();

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  CharacterComponent* m_CharacterComp { nullptr };
  MovementStateMachine* m_MovementStateMachine { nullptr };
};

class FallMovementState : public State {
public:
  FallMovementState(MovementStateMachine* movementStateMachine) :
    State(static_cast<int8_t>(MovementStates::FALL)), m_MovementStateMachine(movementStateMachine) {};

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  bool m_IsCrouchPressed { false };
  bool m_IsJumpPressed { false };

private:
  CharacterComponent* m_CharacterComp { nullptr };
  MovementStateMachine* m_MovementStateMachine { nullptr };
};

class CrouchMovementState : public State {
public:
  CrouchMovementState(MovementStateMachine* movementStateMachine) :
    State(static_cast<int8_t>(MovementStates::CROUCH)), m_MovementStateMachine(movementStateMachine) {};

  void _on_crouch_finished();

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  float m_FinalPos { 0.0f };
  Ref<Tween> m_CrouchTween { nullptr };

private:
  CharacterComponent* m_CharacterComp { nullptr };
  MovementStateMachine* m_MovementStateMachine { nullptr };
};

class SlideMovementState : public State {
public:
  SlideMovementState(MovementStateMachine* movementStateMachine) :
    State(static_cast<int8_t>(MovementStates::SLIDE)), m_MovementStateMachine(movementStateMachine) {};

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

private:
  CharacterComponent* m_CharacterComp { nullptr };
  MovementStateMachine* m_MovementStateMachine { nullptr };
};

class DashMovementState : public State {
public:
  DashMovementState(MovementStateMachine* movementStateMachine) :
    State(static_cast<int8_t>(MovementStates::DASH)), m_MovementStateMachine(movementStateMachine) {};

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _physics_update(double delta) override;
  void _exit() override;

private:
  Vector3 m_DashDir { Vector3(0.0f, 0.0f, 0.0f )};

private:
  CharacterComponent* m_CharacterComp { nullptr };
  MovementStateMachine* m_MovementStateMachine { nullptr };
};
