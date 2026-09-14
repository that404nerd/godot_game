#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include "godot_cpp/classes/global_constants.hpp"
#include "hsm/limbo_state.h"

#include "state.h"
#include "../resources/ai_behaviour_props.h"
#include "../components/ai/ai_character_component.h"
#include "../utils/damped_spring.h"
#include "../input_command_system.h"

using namespace godot;

class AIStateMachine;

// struct AIStateCtx
// {
//   Vector3 LastPlayerPosBeforePatrol = Vector3(0.0f, 0.0f, 0.0f);
//   Vector3 NextNavigationPoint = Vector3(0.0f, 0.0f, 0.0f);
//   Vector3 ToPlayerDirection = Vector3(0.0f, 0.0f, 0.0f);
//   Vector3 AIDirection = Vector3(0.0f, 0.0f, 0.0f);
//   Vector3 AIVelocity = Vector3(0.0f, 0.0f, 0.0f);
//   Vector3 AIMovePosition = Vector3(0.0f, 0.0f, 0.0f);

//   float ToPlayerDistance = 0.0f;

//   bool MovePointAvailable = false;
//   bool TargetForShootReached = false;
//   bool CanSeePlayer = false;

//   bool WantsToIdle = false;
//   bool WantsToShoot = false;
//   bool WantsToChase = false;

//   bool IsNavigationFinished = false;
// };

class BaseAIState : public LimboState {
  GDCLASS(BaseAIState, LimboState);

public:
  void _ready() override;

protected:
  static void _bind_methods();

protected:
  AICharacterComponent* m_AICharacterComp { nullptr };
  AIManager* m_AIManager { nullptr };
  InputCommandSystem* m_InputCmdSystem { nullptr };

private:
  GD_DEFINE_PROPERTY(NodePath, ai_manager_node_path, NodePath());
  GD_DEFINE_PROPERTY(NodePath, ai_character_component_node_path, NodePath());
  GD_DEFINE_PROPERTY(NodePath, input_cmd_system_node_path, NodePath());
};

///////////////////////////// AI States Declaration //////////////////////////////////
class AIIdleState : public BaseAIState {
  GDCLASS(AIIdleState, BaseAIState);
public:
  void _enter() override;
  void _update(double delta) override;

  void _exit() override;
private:
  GD_DEFINE_PROPERTY(NodePath, ai_manager_node_path, NodePath());
  AIManager* m_AIManagerInst { nullptr };
  GD_DEFINE_PROPERTY(AICharacterComponent*, ai_character_component, nullptr);
protected:
  static void _bind_methods() {};

};

class AIMoveState : public BaseAIState {
  GDCLASS(AIMoveState, BaseAIState);
public:
  void _enter() override;
  void _update(double delta) override;

  void _exit() override;
protected:
  static void _bind_methods() {};


};

class AIChaseState : public BaseAIState {
  GDCLASS(AIChaseState, BaseAIState);
public:
  void _enter() override;
  void _update(double delta) override;

  void _exit() override;
protected:
  static void _bind_methods() {};

};

class AIPatrolState : public BaseAIState {
  GDCLASS(AIPatrolState, BaseAIState);
public:
  void _enter() override;
  void _update(double delta) override;

  void _exit() override;
protected:
  static void _bind_methods() {};
};