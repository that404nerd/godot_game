#include "ai_states.h"
#include "../managers/ai_manager.h"

void BaseAIState::_setup()
{
  m_AIManager = get_node<AIManager>(ai_manager_node_path);

  if(m_AIManager)
  {
    m_AICharacterComp = m_AIManager->get_ai_character_component();
    m_InputCmdSystem = m_AIManager->get_input_cmd_system();
    m_AIHsm = m_AICharacterComp->get_ai_state_machine();
    m_AIBehaviourProps = m_AICharacterComp->get_ai_behaviour_props();
    m_BTPlayerInst = m_AICharacterComp->get_bt_player_inst();
    m_Blackboard = m_BTPlayerInst->get_blackboard();
  }
}

LimboState* BaseAIState::get_state(StringName stateName)
{
  Array states = m_AIHsm->get_children();
  LimboState* foundState = Object::cast_to<LimboState>(m_AIHsm->find_child(stateName));

  if(foundState != nullptr && !stateName.is_empty())
    return foundState;
  else return nullptr;
}

void BaseAIState::_bind_methods()
{
  GD_BIND_PROPERTY(BaseAIState, ai_manager_node_path, Variant::NODE_PATH);
  GD_BIND_CUSTOM_PROPERTY(BaseAIState, BehaviorTree, bt_resource, Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Idle AI State /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void AIIdleState::_enter()
{
  m_BTPlayerInst->set_behavior_tree(get_bt_resource());
  m_BTPlayerInst->set_active(true);
  print_line("Going idle!");
}

void AIIdleState::_update(double delta)
{
  float toPlayerDist = m_Blackboard->get_var("ToPlayerDistance");
  bool canSeePlayer = m_Blackboard->get_var("CanSeePlayer");

  if(canSeePlayer)
  {
    m_AIHsm->change_active_state(get_state("Combat"));
  }
}


void AIIdleState::_exit()
{
  m_BTPlayerInst->set_active(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Alert AI State /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void AIAlertState::_enter()
{
  m_BTPlayerInst->set_behavior_tree(get_bt_resource());
  m_BTPlayerInst->set_active(true);

  print_line("Alert!");
}

void AIAlertState::_update(double delta)
{
  float toPlayerDist = m_Blackboard->get_var("ToPlayerDistance");
  bool canSeePlayer = m_Blackboard->get_var("CanSeePlayer");

  // if(canSeePlayer && toPlayerDist >= m_AIBehaviourProps->get_playerDistToTriggerChase())
  // {
  //   m_AIHsm->change_active_state(get_state("Combat"));
  // }
}


void AIAlertState::_exit()
{
  m_BTPlayerInst->set_active(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Chase AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void AICombatState::_enter()
{
  m_BTPlayerInst->set_behavior_tree(get_bt_resource());
  m_BTPlayerInst->set_active(true);

  print_line("Combat!");
}

void AICombatState::_update(double delta)
{

}

void AICombatState::_exit()
{
  m_BTPlayerInst->set_active(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Dead AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void AIDeadState::_enter()
{
  print_line("Dead!");
}

void AIDeadState::_update(double delta)
{
}

void AIDeadState::_exit()
{
}