#include "ai_states.h"
#include "../state_machines/ai_state_machine.h"
#include "../managers/ai_manager.h"

BaseAIState::BaseAIState(AIStates aiState, const AIStateData& aiStateData)
    : State(static_cast<int>(aiState)), m_AIManagerInst(aiStateData.aiManager), m_AIStateMachine(aiStateData.aiStateMachine),
      m_InputCmdSystem(m_AIManagerInst->get_input_cmd_system()), m_AIStateCtxInst(m_AIManagerInst->get_ai_state_ctx()) {};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Idle AI State /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

IdleAIState::IdleAIState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::IDLE, aiStateData)
{

}

void IdleAIState::_enter()
{
  m_InputCmdSystem->set_wants_to_idle(true);
}

void IdleAIState::_handle_input(const Ref<InputEvent>& event)
{

}

void IdleAIState::_update(double delta)
{
  m_AIManagerInst->_idle(delta);  
}

void IdleAIState::_physics_update(double delta)
{

}


void IdleAIState::_exit()
{
  m_InputCmdSystem->set_wants_to_idle(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Chase AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

ChaseAIState::ChaseAIState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::CHASE, aiStateData)
{

}

void ChaseAIState::_enter()
{
  m_InputCmdSystem->set_wants_to_sprint(true);
}

void ChaseAIState::_handle_input(const Ref<InputEvent>& event)
{
  
}

void ChaseAIState::_update(double delta)
{
  m_AIManagerInst->_chase(delta);  

  float toPlayerDist = m_AIStateCtxInst.ToPlayerDistance;

  if(toPlayerDist > 20.0f)
  {
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::PATROL));
  }

  if(m_AIStateCtxInst.IsNavigationFinished)
  {
    print_line("Combat State");
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::IDLE));
  }
}

void ChaseAIState::_physics_update(double delta)
{

}

void ChaseAIState::_exit()
{
  m_InputCmdSystem->set_wants_to_sprint(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Patrol AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

PatrolAIState::PatrolAIState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::PATROL, aiStateData)
{

}

void PatrolAIState::_enter()
{
  m_InputCmdSystem->set_wants_to_walk(true);
  m_AIManagerInst->_patrol_enter();
}

void PatrolAIState::_handle_input(const Ref<InputEvent>& event)
{
  
}

void PatrolAIState::_update(double delta)
{
  m_AIManagerInst->_patrol(delta);  

  if(m_AIStateCtxInst.WantsToChase)
  {
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::CHASE));
  }
}

void PatrolAIState::_physics_update(double delta)
{

}

void PatrolAIState::_exit()
{
  m_InputCmdSystem->set_wants_to_walk(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Combat AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CombatAIState::CombatAIState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::COMBAT, aiStateData)
{

}

void CombatAIState::_enter()
{
  m_InputCmdSystem->set_wants_to_walk(true);
}

void CombatAIState::_handle_input(const Ref<InputEvent>& event)
{
  
}

void CombatAIState::_update(double delta)
{
  // m_AIManagerInst->_(delta);  
  
}

void CombatAIState::_physics_update(double delta)
{

}

void CombatAIState::_exit()
{
  m_InputCmdSystem->set_wants_to_walk(false);
}
