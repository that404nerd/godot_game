#include "ai_states.h"
#include "../state_machines/ai_state_machine.h"

BaseAIState::BaseAIState(AIStates aiState, const AIStateData& aiStateData)
    : State(static_cast<int>(aiState)), m_AIStateMachine(aiStateData.aiStateMachine), m_AICharacterComp(m_AIStateMachine->get_ai_character_component()), 
      m_NavAgent3D(m_AIStateMachine->get_nav_agent_3d()), m_InputCmdSystem(m_AIStateMachine->get_input_cmd_system()),
      m_DetectionArea(m_AIStateMachine->get_detection_area()), m_Target(aiStateData.target) {};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Idle AI State /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

IdleAIState::IdleAIState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::AI_IDLE, aiStateData)
{

}

void IdleAIState::_enter()
{

}

void IdleAIState::_handle_input(const Ref<InputEvent>& event)
{

}

void IdleAIState::_update(double delta)
{

}

void IdleAIState::_physics_update(double delta)
{

}


void IdleAIState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Chase AI State //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

ChaseAIState::ChaseAIState(const AIStateData& aiStateData)
  : BaseAIState(AIStates::AI_CHASE, aiStateData)
{

}

void ChaseAIState::_enter()
{
}

void ChaseAIState::_handle_input(const Ref<InputEvent>& event)
{

}

void ChaseAIState::_update(double delta)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return;
  }
  
  m_NavAgent3D->set_target_position(m_Target->get_global_position());
  if(m_NavAgent3D->is_navigation_finished())
  {
    m_NavAgent3D->set_velocity(Vector3(0.0f, 0.0f, 0.0f));
    return;
  }

  Vector3 nextPos = m_NavAgent3D->get_next_path_position();
  Vector3 direction = (nextPos - m_AICharacterComp->get_global_position()).normalized();

  m_InputCmdSystem->set_wants_to_sprint(true);

  m_AICharacterComp->set_wish_dir(direction);
  m_NavAgent3D->set_velocity(m_AICharacterComp->get_velocity());

  if(direction.length() > 0.01f)
  {
    float target_rot = Math::atan2(direction.x, direction.z);
    Vector3 aiRot = m_AICharacterComp->get_rotation();
    aiRot.y = Math::lerp(aiRot.y, target_rot, 5.0f * (float)delta);
    m_AICharacterComp->set_rotation(aiRot);
  }
}

void ChaseAIState::_physics_update(double delta)
{

}

void ChaseAIState::_exit()
{
  m_InputCmdSystem->set_wants_to_sprint(true);
}