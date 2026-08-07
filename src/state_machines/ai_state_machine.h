#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/animation_tree.hpp>

#include "state_machine.h"
#include "../components/ai_character_component.h"
#include "../input_command_system.h"

using namespace godot;

class IdleAIState;
class ChaseAIState;

enum class AIStates {
  NONE = -1, AI_IDLE, AI_CHASE
};


class AIStateMachine;
struct AIStateData 
{
  AIStateMachine* aiStateMachine;
  Node3D* target;
  // AIManager* aiManager;
};

class AIStateMachine : public StateMachine
{
  GDCLASS(AIStateMachine, StateMachine);
public:
  void _init_data() override;

  void _player_entered_detection_area(Node3D* body);

protected:
  static void _bind_methods();
private:
  AIStateData m_AIStateData;

private:
  GD_DEFINE_PROPERTY(AICharacterComponent*, ai_character_component, nullptr);
  GD_DEFINE_PROPERTY(InputCommandSystem*, input_cmd_system, nullptr);
  GD_DEFINE_PROPERTY(NavigationAgent3D*, nav_agent_3d, nullptr);
  GD_DEFINE_PROPERTY(Area3D*, detection_area, nullptr);
  GD_DEFINE_PROPERTY(AnimationTree*, anim_tree, nullptr);
};