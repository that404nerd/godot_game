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
  GD_BIND_CUSTOM_PROPERTY(AIManager, detection_area, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, anim_player, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void AIManager::_update(double delta)
{
  m_AIStateCtxInst.ToPlayerDistance = (m_Target->get_global_position() - ai_character_component->get_global_position()).length();
  m_AIStateCtxInst.IsNavigationFinished = nav_agent_3d->is_navigation_finished();
}

void AIManager::_physics_update(double delta)
{

}

void AIManager::_idle(double delta)
{
  m_AnimTreeState->travel("Idle");
  nav_agent_3d->set_velocity(ai_character_component->get_velocity());
}

void AIManager::_chase(double delta)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return;
  }

  nav_agent_3d->set_target_position(m_Target->get_global_position());
  
  Vector3 nextPos = nav_agent_3d->get_next_path_position();
  Vector3 direction = (nextPos - ai_character_component->get_global_position()).normalized();
 

  nav_agent_3d->set_target_desired_distance(5.0f);
  
  
  ai_character_component->set_wish_dir(direction);
  nav_agent_3d->set_velocity(ai_character_component->get_velocity());
  m_AnimTreeState->travel("Chase");
  
  if(direction.length() > 0.01f)
  {
    float target_rot = Math::atan2(direction.x, direction.z);
    Vector3 aiRot = ai_character_component->get_rotation();
    aiRot.y = Math::lerp(aiRot.y, target_rot, 5.0f * (float)delta);
    ai_character_component->set_rotation(aiRot);
  }
}

void AIManager::_patrol(double delta)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return;
  }
  
  nav_agent_3d->set_target_position(m_Target->get_global_position());
  
  
  Vector3 nextPos = nav_agent_3d->get_next_path_position();
  Vector3 direction = (nextPos - ai_character_component->get_global_position()).normalized();
   
  ai_character_component->set_wish_dir(direction);
  nav_agent_3d->set_velocity(ai_character_component->get_velocity());
  m_AnimTreeState->travel("Patrol");

  if(direction.length() > 0.01f)
  {
    float target_rot = Math::atan2(direction.x, direction.z);

    // Get the rotation: Math::rad_to_deg(target_to_rot)

    Vector3 aiRot = ai_character_component->get_rotation();
    aiRot.y = Math::lerp(aiRot.y, target_rot, 5.0f * (float)delta);
    ai_character_component->set_rotation(aiRot);
  }
}