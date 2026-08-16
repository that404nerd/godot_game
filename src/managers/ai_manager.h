#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/animation_tree.hpp>

#include "../components/ai_character_component.h"
#include "../state_machines/ai_state_machine.h"
#include "../input_command_system.h"

using namespace godot;

class AIManager : public Node {
  GDCLASS(AIManager, Node);

protected:
  static void _bind_methods();

public:
  void _init();

  void _idle(double delta);
  void _chase(double delta);

  void _patrol_enter();
  void _patrol(double delta);

  void _rotate_character(double delta);

  void _update(double delta);
  void _physics_update(double delta);

public:
  const AIStateCtx& get_ai_state_ctx() { return m_AIStateCtxInst; }

  float get_to_player_dist() { return m_AIStateCtxInst.ToPlayerDistance; }

private:
  GD_DEFINE_PROPERTY(AICharacterComponent*, ai_character_component, nullptr);
  GD_DEFINE_PROPERTY(InputCommandSystem*, input_cmd_system, nullptr);
  GD_DEFINE_PROPERTY(NavigationAgent3D*, nav_agent_3d, nullptr);
  GD_DEFINE_PROPERTY(AnimationTree*, anim_tree, nullptr);
  GD_DEFINE_PROPERTY(AnimationPlayer*, anim_player, nullptr);

private:
  AnimationNodeStateMachinePlayback* m_AnimTreeState { nullptr };
  Player* m_Target { nullptr };

  AIStateCtx m_AIStateCtxInst;
};