#include "ai_state_machine.h"
#include "../managers/ai_manager.h"
#include "../states/ai_states.h"

void AIStateMachine::_init_data()
{
  m_AIStateData = { this, ai_manager_inst };

  m_States[static_cast<int>(AIStates::IDLE)] = std::make_unique<IdleAIState>(m_AIStateData);
  m_States[static_cast<int>(AIStates::CHASE)] = std::make_unique<ChaseAIState>(m_AIStateData);
  m_States[static_cast<int>(AIStates::PATROL)] = std::make_unique<PatrolAIState>(m_AIStateData);

  m_InitialState = m_States.at(static_cast<int>(AIStates::IDLE)).get();

  // detection_area->connect("body_entered", Callable(this, "_player_entered_detection_area"));
}

void AIStateMachine::_player_entered_detection_area(Node3D* body)
{
  // _change_state(static_cast<int>(AIStates::PATROL));
}

void AIStateMachine::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_player_entered_detection_area", "body"), &AIStateMachine::_player_entered_detection_area);

  GD_BIND_CUSTOM_PROPERTY(AIStateMachine, ai_manager_inst, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}