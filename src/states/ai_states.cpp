#include "ai_states.h"
#include "../state_machines/ai_state_machine.h"
#include "../managers/ai_manager.h"

BaseAIState::BaseAIState(AIStates aiState, const AIStateData& aiStateData)
    : State(static_cast<int>(aiState)), m_AIManagerInst(aiStateData.aiManager), m_AIStateMachine(aiStateData.aiStateMachine),
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

  if(toPlayerDist <= 10.0f)
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Combat AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

AICombatState::AICombatState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::COMBAT, aiStateData)
{

}

void AICombatState::_enter()
{
  m_CombatStates[static_cast<int>(AICombatStates::SHOOT)] = std::make_unique<AIShootState>(AIStateData{
    .aiStateMachine = m_AIStateMachine,
    .aiManager = m_AIManagerInst
  });

  m_CombatStates[static_cast<int>(AICombatStates::RELOAD)] = std::make_unique<AIReloadState>(AIStateData{
    .aiStateMachine = m_AIStateMachine,
    .aiManager = m_AIManagerInst
  });

  m_InitialState = m_CombatStates.at(static_cast<int>(AICombatStates::SHOOT)).get();

  if(m_InitialState) {
    m_InitialState->_enter();
    m_CurrentState = m_InitialState;
  }
}

void AICombatState::_handle_input(const Ref<InputEvent>& event)
{
  if(m_CurrentState) {
    m_CurrentState->_handle_input(event);
  } 
}

void AICombatState::_update(double delta)
{
  if(m_CurrentState)
    m_CurrentState->_update(delta);   
}

void AICombatState::_physics_update(double delta)
{
  if(m_CurrentState) {
    m_CurrentState->_physics_update(delta);
  }
}

void AICombatState::_exit()
{
  if(m_CurrentState) {
    m_CurrentState->_exit();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Shoot AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

AIShootState::AIShootState(const AIStateData& aiStateData)
  : AICombatState(aiStateData)
{

}

void AIShootState::_enter()
{
  m_InputCmdSystem->set_wants_to_idle(true);
  print_line("Gonna shoot!");
}

void AIShootState::_handle_input(const Ref<InputEvent>& event)
{
  
}

void AIShootState::_update(double delta)
{
  m_AIManagerInst->_shoot(delta);

  float toPlayerDist = m_AIStateCtxInst.ToPlayerDistance;

  if(toPlayerDist >= 15.0f)
  {
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::CHASE));
  }

  if(!m_AIStateCtxInst.CanSeePlayer)
    m_AIStateMachine->_change_state(static_cast<int>(AIStates::PATROL));
}

void AIShootState::_physics_update(double delta)
{

}

void AIShootState::_exit()
{
  m_InputCmdSystem->set_wants_to_idle(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Reload AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

AIReloadState::AIReloadState(const AIStateData& aiStateData)
  : AICombatState(aiStateData)
{

}

void AIReloadState::_enter()
{
}

void AIReloadState::_handle_input(const Ref<InputEvent>& event)
{
  
}

void AIReloadState::_update(double delta)
{
 
}

void AIReloadState::_physics_update(double delta)
{

}

void AIReloadState::_exit()
{
}