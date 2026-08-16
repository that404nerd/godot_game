#include "ai_manager.h"
#include "../components/ai_character_component.h"

void AIManager::_init()
{
  m_AnimTreeState = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/playback"));
  m_Target = Object::cast_to<Player>(get_tree()->get_first_node_in_group("player"));
}

void AIManager::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(AIManager, ai_character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, input_cmd_system, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, nav_agent_3d, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, anim_player, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void AIManager::_update(double delta)
{
  m_AIStateCtxInst.ToPlayerDistance = (m_Target->get_global_position() - ai_character_component->get_global_position()).length();

  m_AIStateCtxInst.NextNavigationPoint = nav_agent_3d->get_next_path_position();

  m_AIStateCtxInst.AIDirection = (m_AIStateCtxInst.NextNavigationPoint - ai_character_component->get_global_position()).normalized();

  _rotate_character(delta);

  ai_character_component->set_wish_dir(m_AIStateCtxInst.AIDirection);
  nav_agent_3d->set_velocity(ai_character_component->get_velocity());
}

void AIManager::_physics_update(double delta)
{

}

void AIManager::_rotate_character(double delta)
{
  if(m_AIStateCtxInst.AIDirection.length() > 0.01f)
  {
    float target_rot = Math::atan2(m_AIStateCtxInst.AIDirection.x, m_AIStateCtxInst.AIDirection.z);
    Vector3 aiRot = ai_character_component->get_rotation();
    aiRot.y = Math::lerp(aiRot.y, target_rot, 5.0f * (float)delta);
    ai_character_component->set_rotation(aiRot);
  }
}

void AIManager::_idle(double delta)
{
  m_AIStateCtxInst.IsNavigationFinished = false;
  m_AnimTreeState->travel("Idle");
}

void AIManager::_chase(double delta)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return;
  }

  m_AIStateCtxInst.IsNavigationFinished = nav_agent_3d->is_navigation_finished();

  m_AnimTreeState->travel("Chase");
  nav_agent_3d->set_target_position(m_Target->get_global_position());
  nav_agent_3d->set_target_desired_distance(5.0f);
}

void AIManager::_patrol_enter()
{
  m_AIStateCtxInst.LastPlayerPosBeforePatrol = m_Target->get_global_position();
}

void AIManager::_patrol(double delta)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return;
  }
  
  m_AnimTreeState->travel("Patrol");
  nav_agent_3d->set_target_position(m_Target->get_global_position());
}