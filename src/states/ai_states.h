#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <unordered_map>

#include "state.h"
#include "../resources/ai_behaviour_props.h"
#include "../components/ai/ai_character_component.h"
#include "../utils/damped_spring.h"
#include "../input_command_system.h"

class AIStateMachine;
enum class AIStates;
struct AIStateData;
class AIStateMachine;

using namespace godot;

struct AIStateCtx
{
  Vector3 LastPlayerPosBeforePatrol = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 NextNavigationPoint = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 ToPlayerDirection = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 AIDirection = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 AIVelocity = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 AIMovePosition = Vector3(0.0f, 0.0f, 0.0f);

  float ToPlayerDistance = 0.0f;

  bool MovePointAvailable = false;
  bool TargetForShootReached = false;
  bool CanSeePlayer = false;

  bool WantsToIdle = false;
  bool WantsToShoot = false;
  bool WantsToChase = false;

  bool IsNavigationFinished = false;
};

class BaseAIState : public State {
public:
  BaseAIState(AIStates aiState, const AIStateData& aiStateData);
protected:
  AIManager* m_AIManagerInst { nullptr };
  AIStateMachine* m_AIStateMachine { nullptr };

  AICharacterComponent* m_AICharacterComp { nullptr };
  Ref<AIBehaviourProps> m_AIBehaviourProps { nullptr };

  InputCommandSystem* m_InputCmdSystem { nullptr };
  const AIStateCtx& m_AIStateCtxInst;
};

///////////////////////////// AI States Declaration //////////////////////////////////
class AIIdleState : public BaseAIState {
public:
  AIIdleState(const AIStateData& aiStateData); 

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _physics_update(double delta) override;

  void _exit() override;
};

class AIMoveState : public BaseAIState {
public:
  AIMoveState(const AIStateData& aiStateData); 

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _physics_update(double delta) override;

  void _exit() override;
};

class AIChaseState : public BaseAIState {
public:
  AIChaseState(const AIStateData& aiStateData); 

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _physics_update(double delta) override;

  void _exit() override;
};

class AIPatrolState : public BaseAIState {
public:
  AIPatrolState(const AIStateData& aiStateData); 

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _physics_update(double delta) override;

  void _exit() override;

};