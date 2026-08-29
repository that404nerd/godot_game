#include "ai_manager.h"
#include "../components/ai/ai_character_component.h"

void AIManager::_init()
{
  m_AnimTreeState = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/playback"));
  m_CombatTreeState = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/Combat/playback"));
  m_PatrolTreeState = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/Patrol/playback"));
  m_ChaseTreeState = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/Chase/playback"));
  m_NormalChaseTreeState = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/Chase/CrouchChase/playback"));
  m_CrouchChaseTreeState = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/Chase/NormalChase/playback"));

  m_Target = Object::cast_to<Player>(get_tree()->get_first_node_in_group("player"));

  if(ai_vision_component)
    ai_vision_component->_init();

  if(lookat_player_component)
    lookat_player_component->_init(m_Target);
  else {
    print_error("Look at player component is null!");
    return;
  }
}

void AIManager::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(AIManager, AICharacterComponent, ai_character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, InputCommandSystem, input_cmd_system, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, NavigationAgent3D, nav_agent_3d, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, AnimationPlayer, anim_player, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, AnimationTree, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, LookAtPlayerComponent, lookat_player_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, VisionComponent, ai_vision_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void AIManager::_update(double delta)
{
  if(ai_vision_component)
    ai_vision_component->_update(delta);

  if(lookat_player_component)
    lookat_player_component->_update(delta);

  m_AIStateCtxInst.AIVelocity = ai_character_component->get_velocity();
  m_AIStateCtxInst.ToPlayerDistance = (m_Target->get_global_position() - ai_character_component->get_global_position()).length();
  m_AIStateCtxInst.NextNavigationPoint = nav_agent_3d->get_next_path_position();

  m_AIStateCtxInst.ToPlayerDirection = m_Target->get_global_position() - ai_character_component->get_global_position();
  m_AIStateCtxInst.AIDirection = (m_AIStateCtxInst.NextNavigationPoint - ai_character_component->get_global_position()).normalized();

  if(ai_vision_component)
    m_AIStateCtxInst.CanSeePlayer = ai_vision_component->can_see_player();


  ai_character_component->set_wish_dir(m_AIStateCtxInst.AIDirection);
  nav_agent_3d->set_velocity(ai_character_component->get_velocity());
}

void AIManager::_physics_update(double delta)
{
  if(ai_vision_component)
    ai_vision_component->_physics_update(delta);
}

void AIManager::_rotate_character(double delta)
{
  if(m_AIStateCtxInst.ToPlayerDirection.length() > 0.01f)
  {
    Vector3 enemyForward = (ai_character_component->get_basis().get_column(2)).normalized();
    Vector3 toTarget = (m_Target->get_global_position() - ai_character_component->get_global_position()).normalized();

    enemyForward.y = 0.0f;
    toTarget.y = 0.0f;

    float angle = enemyForward.angle_to(toTarget);
    Vector3 toFace = enemyForward.cross(toTarget);

    if(toFace.y < 0.0f)
      angle = -angle;

    ai_character_component->rotate(Vector3(0.0f, 1.0f, 0.0f), angle);
  }

}

void AIManager::_idle(double delta)
{
  lookat_player_component->set_look_status(false);

  m_AIStateCtxInst.IsNavigationFinished = false;
  m_AnimTreeState->travel("Idle");
}

void AIManager::_blend_chase_states(double delta)
{
  Vector2 dir = Vector2(m_AIStateCtxInst.AIDirection.x, m_AIStateCtxInst.AIDirection.z).normalized();
  anim_tree->set("parameters/Chase/blend_position", dir);
}

void AIManager::_chase(double delta)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return;
  }

  _blend_chase_states(delta);
  _rotate_character(delta);
  lookat_player_component->set_look_status(true);

  m_AIStateCtxInst.IsNavigationFinished = nav_agent_3d->is_navigation_finished();

  m_AnimTreeState->travel("Chase");
  nav_agent_3d->set_target_position(m_Target->get_global_position());
  nav_agent_3d->set_target_desired_distance(5.0f);

}

void AIManager::_patrol_enter()
{
  m_AIStateCtxInst.LastPlayerPosBeforePatrol = m_Target->get_global_position();
}

void AIManager::_blend_patrol_states(double delta)
{
  Vector2 dir = Vector2(m_AIStateCtxInst.AIDirection.x, m_AIStateCtxInst.AIDirection.z).normalized();
  anim_tree->set("parameters/Patrol/blend_position", dir);
}

void AIManager::_patrol(double delta)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return;
  }
  
  _blend_patrol_states(delta);
  lookat_player_component->set_look_status(false);
  m_AnimTreeState->travel("Patrol");

  nav_agent_3d->set_target_position(m_AIStateCtxInst.LastPlayerPosBeforePatrol);

  if(nav_agent_3d->is_navigation_finished())
  {
    m_AIStateCtxInst.IsNavigationFinished = true;
  }

}

void AIManager::_shoot(double delta)
{
  _rotate_character(delta);

  m_AnimTreeState->travel("Combat");
  m_CombatTreeState->travel("Enemy_Stand_Shoot");

  lookat_player_component->set_look_status(true);
}