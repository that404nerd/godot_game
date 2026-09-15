#include "ai_states.h"
#include "../managers/ai_manager.h"

void BaseAIState::_setup()
{
  ERR_FAIL_COND_MSG(get_parent()->get_class() != StringName("LimboHSM"), "Parent node must be of type LimboHSM");
  m_AIManager = get_node<AIManager>(ai_manager_node_path);

  if(m_AIManager)
  {
    print_line("Initialized");
    m_AICharacterComp = m_AIManager->get_ai_character_component();
    m_InputCmdSystem = m_AIManager->get_input_cmd_system();
    m_AIHsm = m_AICharacterComp->get_ai_state_machine();
    m_AIBehaviourProps = m_AICharacterComp->get_ai_behaviour_props();
    m_BTPlayerInst = m_AICharacterComp->get_bt_player_inst();
    m_BlackboardPlan = m_BTPlayerInst->get_blackboard_plan();
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
  m_InputCmdSystem->set_wants_to_idle(true);
  m_BTPlayerInst->set_behavior_tree(get_bt_resource());
  m_BTPlayerInst->set_active(true);
  print_line("Going idle!");
}

void AIIdleState::_update(double delta)
{
  m_AIManager->_idle(delta);  

  float toPlayerDist = m_BlackboardPlan->get_var("ToPlayerDistance").get_value();
  bool canSeePlayer = m_BlackboardPlan->get_var("CanSeePlayer").get_value();
  // bool wantsToShoot = m_BlackboardPlan->get_var("WantsToShoot").get_value();

  if(canSeePlayer && toPlayerDist >= m_AIBehaviourProps->get_playerDistToTriggerChase())
  {
    m_AIHsm->change_active_state(get_state("Chase"));
  }
  
  // if(m_AIStateCtxInst.MovePointAvailable && m_AIStateCtxInst.WantsToShoot)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::MOVE));
  // }
}


void AIIdleState::_exit()
{
  m_InputCmdSystem->set_wants_to_idle(false);
  m_BTPlayerInst->set_active(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Move AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void AIMoveState::_enter()
{
  // get_input_cmd_system()->set_wants_to_walk(true);
  print_line("Gonna move!");
}

void AIMoveState::_update(double delta)
{
  // get_ai_manager()->_move(delta);  

}

void AIMoveState::_exit()
{
  // get_input_cmd_system()->set_wants_to_walk(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Chase AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void AIChaseState::_enter()
{
  // get_input_cmd_system()->set_wants_to_sprint(true);
  print_line("Gonna chase!");
}


void AIChaseState::_update(double delta)
{
  // get_ai_manager()->_chase(delta);  

  // if(!m_AIStateCtxInst.CanSeePlayer)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::PATROL));
  // }

  // if(m_AIStateCtxInst.TargetForShootReached)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::IDLE));
  // }
}

void AIChaseState::_exit()
{
  // get_input_cmd_system()->set_wants_to_sprint(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Patrol AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void AIPatrolState::_enter()
{
  // m_InputCmdSystem->set_wants_to_walk(true);
  // m_AIManagerInst->_patrol_enter();

  print_line("Patrolling!");
}

void AIPatrolState::_update(double delta)
{
  // m_AIManagerInst->_patrol(delta);  

  // if(m_AIStateCtxInst.CanSeePlayer)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::CHASE));
  // }

  // if(m_AIStateCtxInst.IsNavigationFinished)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::IDLE));
  // }
}

void AIPatrolState::_exit()
{
  // m_InputCmdSystem->set_wants_to_walk(false);
}