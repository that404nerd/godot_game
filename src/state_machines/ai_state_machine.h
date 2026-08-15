#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/animation_tree.hpp>

#include "state_machine.h"
#include "../components/ai_character_component.h"
#include "../input_command_system.h"

#include "../states/ai_states.h"

using namespace godot;

class IdleAIState;
class ChaseAIState;
class AIStateMachine;

enum class AIStates {
  NONE = -1, IDLE, CHASE, PATROL, COMBAT
};


struct AIStateData 
{
  AIStateMachine* aiStateMachine;
  AIManager* aiManager;
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

  GD_DEFINE_PROPERTY(AIManager*, ai_manager_inst, nullptr);
};