#include "ai_states.h"
#include "../managers/ai_manager.h"

void BaseAIState::_ready()
{
  m_AICharacterComp = get_node<AICharacterComponent>(ai_character_component_node_path);
  m_AIManager = get_node<AIManager>(ai_manager_node_path);
  m_InputCmdSystem = get_node<InputCommandSystem>(input_cmd_system_node_path);
}

void BaseAIState::_bind_methods()
{
  GD_BIND_PROPERTY(BaseAIState, ai_character_component_node_path, Variant::NODE_PATH);
  GD_BIND_PROPERTY(BaseAIState, ai_manager_node_path, Variant::NODE_PATH);
  GD_BIND_PROPERTY(BaseAIState, input_cmd_system_node_path, Variant::NODE_PATH);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Idle AI State /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void AIIdleState::_enter()
{
  m_InputCmdSystem->set_wants_to_idle(true);
  print_line("Going idle!");
}

void AIIdleState::_update(double delta)
{
  // m_AIManagerInst->_idle(delta);  

  // float toPlayerDist = m_AIStateCtxInst.ToPlayerDistance;

  // if((m_AIStateCtxInst.CanSeePlayer && !m_AIStateCtxInst.WantsToShoot) || 
  //    (m_AIStateCtxInst.WantsToShoot && toPlayerDist >= m_AIBehaviourProps->get_playerDistToTriggerChase()))
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::CHASE));
  // }
  
  // if(m_AIStateCtxInst.MovePointAvailable && m_AIStateCtxInst.WantsToShoot)
  // {
  //   m_AIStateMachine->_change_state(static_cast<int>(AIStates::MOVE));
  // }
}


void AIIdleState::_exit()
{
  // m_InputCmdSystem->set_wants_to_idle(false);
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