#include "ai_state_machine.h"
#include "../states/ai_states.h"

void AIStateMachine::_init_data()
{
  m_AIStateData = { this, Object::cast_to<Node3D>(get_tree()->get_first_node_in_group("player")) };

  m_States[static_cast<int>(AIStates::AI_IDLE)] = std::make_unique<IdleAIState>(m_AIStateData);
  m_States[static_cast<int>(AIStates::AI_CHASE)] = std::make_unique<ChaseAIState>(m_AIStateData);

  m_InitialState = m_States.at(static_cast<int>(AIStates::AI_IDLE)).get();

  detection_area->connect("body_entered", Callable(this, "_player_entered_detection_area"));
}

void AIStateMachine::_player_entered_detection_area(Node3D* body)
{
  _change_state(static_cast<int>(AIStates::AI_CHASE));

}

void AIStateMachine::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_player_entered_detection_area", "body"), &AIStateMachine::_player_entered_detection_area);

  GD_BIND_CUSTOM_PROPERTY(AIStateMachine, ai_character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIStateMachine, input_cmd_system, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIStateMachine, nav_agent_3d, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIStateMachine, detection_area, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIStateMachine, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}