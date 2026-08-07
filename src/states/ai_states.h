#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include "state.h"
#include "../state_machines/ai_state_machine.h"
#include "../components/ai_character_component.h"
#include "../input_command_system.h"

class AIStateMachine;

using namespace godot;

class AIStateMachine;

class BaseAIState : public State {
public:
  BaseAIState(AIStates aiState, const AIStateData& aiStateData);
protected:
  AIStateMachine* m_AIStateMachine { nullptr };
  AICharacterComponent* m_AICharacterComp { nullptr };
  NavigationAgent3D* m_NavAgent3D { nullptr };
  InputCommandSystem* m_InputCmdSystem { nullptr };
  Area3D* m_DetectionArea { nullptr };
  Node3D* m_Target { nullptr };
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