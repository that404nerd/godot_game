#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include "blackboard/blackboard_plan.h"
#include "bt/behavior_tree.h"
#include "bt/bt_player.h"
#include "hsm/limbo_state.h"

#include "state.h"
#include "../resources/ai_behaviour_props.h"
#include "../components/ai/ai_character_component.h"
#include "../utils/damped_spring.h"
#include "../input_command_system.h"

using namespace godot;

class AIStateMachine;

class BaseAIState : public LimboState {
  GDCLASS(BaseAIState, LimboState);

public:
  void _setup() override;

protected:
  LimboState* get_state(StringName state_name);

  static void _bind_methods();

protected:
  AICharacterComponent* m_AICharacterComp { nullptr };
  AIManager* m_AIManager { nullptr };
  InputCommandSystem* m_InputCmdSystem { nullptr };
  BTPlayer* m_BTPlayerInst { nullptr };
  LimboHSM* m_AIHsm { nullptr };
  Ref<AIBehaviourProps> m_AIBehaviourProps { nullptr };
  Ref<BlackboardPlan> m_BlackboardPlan { nullptr };

private:
  GD_DEFINE_PROPERTY(NodePath, ai_manager_node_path, NodePath());
  GD_DEFINE_PROPERTY(Ref<BehaviorTree>, bt_resource, nullptr);
};

///////////////////////////// AI States Declaration //////////////////////////////////
class AIIdleState : public BaseAIState {
  GDCLASS(AIIdleState, BaseAIState);
public:
  void _enter() override;
  void _update(double delta) override;

  void _exit() override;
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