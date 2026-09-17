#include "ai_states.h"
#include "../managers/ai_manager.h"

void BaseAIState::_setup()
{
  // ERR_FAIL_COND_MSG(get_parent()->get_class() != StringName("LimboHSM"), "Parent node must be of type LimboHSM");
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

  // if(canSeePlayer && toPlayerDist >= m_AIBehaviourProps->get_playerDistToTriggerChase())
  // {
  //   m_AIHsm->change_active_state(get_state("Combat"));
  // }
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
  print_line("Going idle!");
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
}

void AICombatState::_update(double delta)
{
  // if(!m_AIStateCtxInst.CanSeePlayer)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::PATROL));
  // }

  // if(m_AIStateCtxInst.TargetForShootReached)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::IDLE));
  // }
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
  // m_InputCmdSystem->set_wants_to_walk(true);
  // m_AIManager->_patrol_enter();

  print_line("Patrolling!");
}

void AIDeadState::_update(double delta)
{
  // m_AIManager->_patrol(delta);  

  // if(m_AIStateCtxInst.CanSeePlayer)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::CHASE));
  // }

  // if(m_AIStateCtxInst.IsNavigationFinished)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::IDLE));
  // }
}

void AIDeadState::_exit()
{
  // m_InputCmdSystem->set_wants_to_walk(false);
}