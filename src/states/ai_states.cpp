#include "ai_states.h"
#include "../state_machines/ai_state_machine.h"
#include "../managers/ai_manager.h"

BaseAIState::BaseAIState(AIStates aiState, const AIStateData& aiStateData)
    : State(static_cast<int>(aiState)), m_AIManagerInst(aiStateData.aiManager), m_AIStateMachine(aiStateData.aiStateMachine),
      m_AICharacterComp(m_AIManagerInst->get_ai_character_component()), m_AIBehaviourProps(m_AICharacterComp->get_ai_behaviour_props()),
      m_InputCmdSystem(m_AIManagerInst->get_input_cmd_system()), m_AIStateCtxInst(m_AIManagerInst->get_ai_state_ctx()) {};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Idle AI State /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

AIIdleState::AIIdleState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::IDLE, aiStateData)
{

}

void AIIdleState::_enter()
{
  m_InputCmdSystem->set_wants_to_idle(true);
  print_line("Going idle! Player not visible!");
}

void AIIdleState::_handle_input(const Ref<InputEvent>& event)
{

}

void AIIdleState::_update(double delta)
{
  m_AIManagerInst->_idle(delta);  

  if(m_AIStateCtxInst.CanSeePlayer)
  {
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::CHASE));
  }
}

void AIIdleState::_physics_update(double delta)
{

}


void AIIdleState::_exit()
{
  m_InputCmdSystem->set_wants_to_idle(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Chase AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

AIChaseState::AIChaseState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::CHASE, aiStateData)
{

}

void AIChaseState::_enter()
{
  m_InputCmdSystem->set_wants_to_sprint(true);
  print_line("Gonna chase!");
}

void AIChaseState::_handle_input(const Ref<InputEvent>& event)
{
  
}

void AIChaseState::_update(double delta)
{
  m_AIManagerInst->_chase(delta);  

  float toPlayerDist = m_AIStateCtxInst.ToPlayerDistance;

  if(toPlayerDist <= m_AIBehaviourProps->get_enemyDistToTriggerCombat() && m_AIStateCtxInst.CanSeePlayer)
  {
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::COMBAT));
  }

  if(!m_AIStateCtxInst.CanSeePlayer)
  {
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::PATROL));
  }
}

void AIChaseState::_physics_update(double delta)
{

}

void AIChaseState::_exit()
{
  m_InputCmdSystem->set_wants_to_sprint(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Patrol AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

AIPatrolState::AIPatrolState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::PATROL, aiStateData)
{

}

void AIPatrolState::_enter()
{
  m_InputCmdSystem->set_wants_to_walk(true);
  m_AIManagerInst->_patrol_enter();

  print_line("Patrolling!");
}

void AIPatrolState::_handle_input(const Ref<InputEvent>& event)
{
  
}

void AIPatrolState::_update(double delta)
{
  m_AIManagerInst->_patrol(delta);  

  if(m_AIStateCtxInst.CanSeePlayer)
  {
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::CHASE));
  }

  if(m_AIStateCtxInst.IsNavigationFinished)
  {
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::IDLE));
  }
}

void AIPatrolState::_physics_update(double delta)
{

}

void AIPatrolState::_exit()
{
  m_InputCmdSystem->set_wants_to_walk(false);
}