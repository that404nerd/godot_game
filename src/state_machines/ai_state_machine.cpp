#include "ai_state_machine.h"
#include "../managers/ai_manager.h"
#include "../states/ai_states.h"

void AIStateMachine::_init_data()
{
  m_AIStateData = { this, ai_manager_inst };

  m_States[static_cast<int>(AIStates::IDLE)] = std::make_unique<AIIdleState>(m_AIStateData);
  m_States[static_cast<int>(AIStates::CHASE)] = std::make_unique<AIChaseState>(m_AIStateData);
  m_States[static_cast<int>(AIStates::PATROL)] = std::make_unique<AIPatrolState>(m_AIStateData);
  m_States[static_cast<int>(AIStates::COMBAT)] = std::make_unique<AICombatState>(m_AIStateData);

  m_InitialState = m_States.at(static_cast<int>(AIStates::IDLE)).get();
}

void AIStateMachine::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(AIStateMachine, AIManager, ai_manager_inst, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}