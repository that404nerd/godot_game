#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include "state.h"
#include "../components/ai_character_component.h"
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

  float ToPlayerDistance = 0.0f;
  bool IsNavigationFinished = false;
};

class BaseAIState : public State {
public:
  BaseAIState(AIStates aiState, const AIStateData& aiStateData);
protected:
  AIManager* m_AIManagerInst { nullptr };
  AIStateMachine* m_AIStateMachine { nullptr };
  InputCommandSystem* m_InputCmdSystem { nullptr };
  const AIStateCtx& m_AIStateCtxInst;
};

///////////////////////////// AI States Declaration //////////////////////////////////
class IdleAIState : public BaseAIState {
public:
  IdleAIState(const AIStateData& aiStateData); 

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _physics_update(double delta) override;

  void _exit() override;
};

class ChaseAIState : public BaseAIState {
public:
  ChaseAIState(const AIStateData& aiStateData); 

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _physics_update(double delta) override;

  void _exit() override;
};

class PatrolAIState : public BaseAIState {
public:
  PatrolAIState(const AIStateData& aiStateData); 

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _physics_update(double delta) override;

  void _exit() override;

};

class CombatAIState : public BaseAIState {
public:
  CombatAIState(const AIStateData& aiStateData); 

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _physics_update(double delta) override;

  void _exit() override;

};