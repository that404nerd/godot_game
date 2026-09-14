#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/animation_tree.hpp>

#include "../components/ai/ai_character_component.h"
#include "../state_machines/ai_state_machine.h"
#include "../input_command_system.h"
#include "../components/ai/lookat_player_component.h"
#include "../components/ai/vision_component.h"

#include "../geqo/environment_query3d.h"

using namespace godot;

class AIManager : public Node {
  GDCLASS(AIManager, Node);

protected:
  static void _bind_methods();

public:
  AIManager();

  void _init();

  void _idle(double delta);
  void _chase(double delta);

  void _patrol_enter();

  void _blend_patrol_states(double delta);
  void _blend_chase_states(double delta);

  void _patrol(double delta);

  void _shoot(double delta);

  void _rotate_character(double delta);

  void _update(double delta);
  void _physics_update(double delta);

public:
  const AIStateCtx& get_ai_state_ctx() { return m_AIStateCtxInst; }

  void _on_query_finished(QueryResult3D* queryResult);

  float get_to_player_dist() { return m_AIStateCtxInst.ToPlayerDistance; }

private:

  float m_QueryTimer { 0.0f };

  GD_DEFINE_PROPERTY(AICharacterComponent*, ai_character_component, nullptr);
  GD_DEFINE_PROPERTY(InputCommandSystem*, input_cmd_system, nullptr);
  GD_DEFINE_PROPERTY(EnvironmentQuery3D*, env_query3d, nullptr);
  GD_DEFINE_PROPERTY(NavigationAgent3D*, nav_agent_3d, nullptr);
  GD_DEFINE_PROPERTY(AnimationTree*, anim_tree, nullptr);
  GD_DEFINE_PROPERTY(LookAtPlayerComponent*, lookat_player_component, nullptr);
  GD_DEFINE_PROPERTY(VisionComponent*, ai_vision_component, nullptr);

  Ref<AIBehaviourProps> m_AIBehaviourProps { nullptr };

private:
  AnimationNodeStateMachinePlayback *m_LowerBodyStateMachine { nullptr };
  AnimationNodeStateMachinePlayback *m_UpperBodyStateMachine { nullptr };

  Player* m_Target { nullptr };

  AIStateCtx m_AIStateCtxInst;
};