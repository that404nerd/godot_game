#include "ai_manager.h"
#include "../components/ai/ai_character_component.h"

void AIManager::_init()
{
  m_LowerBodyStateMachine = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/LowerBodyStateMachine/playback"));
  m_UpperBodyStateMachine = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/UpperBodyStateMachine/playback"));



  m_AIBehaviourProps = ai_character_component->get_ai_behaviour_props();

  m_Target = Object::cast_to<Player>(get_tree()->get_first_node_in_group("player"));
  env_query3d->connect("query_finished", Callable(this, "_on_query_finished"));

  if(ai_vision_component)
    ai_vision_component->_init();

  if(lookat_player_component)
    lookat_player_component->_init(m_Target);
  else {
    print_error("Look at player component is null!");
    return;
  }

  nav_agent_3d->connect("target_reached", Callable(this, "_target_reached"));
}

void AIManager::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_query_finished", "queryResult"), &AIManager::_on_query_finished);
  ClassDB::bind_method(D_METHOD("_target_reached"), &AIManager::_target_reached);

  GD_BIND_CUSTOM_PROPERTY(AIManager, AICharacterComponent, ai_character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, InputCommandSystem, input_cmd_system, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, NavigationAgent3D, nav_agent_3d, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, EnvironmentQuery3D, env_query3d, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, AnimationTree, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, LookAtPlayerComponent, lookat_player_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, VisionComponent, ai_vision_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void AIManager::_target_reached()
{
  // m_AIStateCtxInst.TargetForShootReached = true;
  // m_AIStateCtxInst.MovePointAvailable = true;
}

void AIManager::_on_query_finished(QueryResult3D* queryResult)
{
  Vector3 best_pos = queryResult->get_highest_score_position();

  // m_AIStateCtxInst.AIMovePosition = best_pos;
}

void AIManager::_update(double delta)
{
  if(ai_vision_component)
    ai_vision_component->_update(delta);

  if(lookat_player_component)
    lookat_player_component->_update(delta);

  // m_AIStateCtxInst.AIVelocity = ai_character_component->get_velocity();
  // m_AIStateCtxInst.ToPlayerDistance = (m_Target->get_global_position() - ai_character_component->get_global_position()).length();
  // m_AIStateCtxInst.NextNavigationPoint = nav_agent_3d->get_next_path_position();

  // m_AIStateCtxInst.ToPlayerDirection = m_Target->get_global_position() - ai_character_component->get_global_position();
  // m_AIStateCtxInst.AIDirection = (m_AIStateCtxInst.NextNavigationPoint - ai_character_component->get_global_position()).normalized();

  // if(ai_vision_component)
  //   m_AIStateCtxInst.CanSeePlayer = ai_vision_component->can_see_player();


  // ai_character_component->set_wish_dir(m_AIStateCtxInst.AIDirection);
  nav_agent_3d->set_velocity(ai_character_component->get_velocity());

  
}

void AIManager::_physics_update(double delta)
{
  if(ai_vision_component)
    ai_vision_component->_physics_update(delta);
}

void AIManager::_rotate_character(double delta)
{
  // if(m_AIStateCtxInst.ToPlayerDirection.length() > 0.01f)
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
  // float toPlayerDist = m_AIStateCtxInst.ToPlayerDistance;

  lookat_player_component->set_look_status(false);

  // m_AIStateCtxInst.IsNavigationFinished = false;
  // m_LowerBodyStateMachine->travel("Idle");

  // if(m_AIStateCtxInst.WantsToShoot)
  // {
  //   _rotate_character(delta);
  //   lookat_player_component->set_look_status(false);
  //   _activate_shoot_state(delta);
  // } else if(toPlayerDist >= m_AIBehaviourProps->get_playerDistToTriggerChase())
  // {
  //   m_AIStateCtxInst.TargetForShootReached = false;
  //   _deactivate_shoot_state(delta);
  // }
}

void AIManager::_blend_chase_states(double delta)
{
  // Vector2 dir = Vector2(m_AIStateCtxInst.AIDirection.x, m_AIStateCtxInst.AIDirection.z).normalized();
  // anim_tree->set("parameters/LowerBodyStateMachine/Run/blend_position", dir);
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

  // float toPlayerDist = m_AIStateCtxInst.ToPlayerDistance;
  // m_AIStateCtxInst.IsNavigationFinished = nav_agent_3d->is_navigation_finished();

  m_LowerBodyStateMachine->travel("Run");
  nav_agent_3d->set_target_position(m_Target->get_global_position());
  nav_agent_3d->set_target_desired_distance(m_AIBehaviourProps->get_enemyDistBetweenPlayer());

  // if(toPlayerDist <= m_AIBehaviourProps->get_enemyDistToShoot() && m_AIStateCtxInst.CanSeePlayer)
  // {
  //   _activate_shoot_state(delta);
  // } else if(toPlayerDist >= m_AIBehaviourProps->get_playerDistToTriggerChase()) {
  //   _deactivate_shoot_state(delta);
  // }
}

void AIManager::_patrol_enter()
{
  // m_AIStateCtxInst.LastPlayerPosBeforePatrol = m_Target->get_global_position();
}

void AIManager::_blend_patrol_states(double delta)
{
  // Vector2 dir = Vector2(m_AIStateCtxInst.AIDirection.x, m_AIStateCtxInst.AIDirection.z).normalized();
  // anim_tree->set("parameters/Patrol/blend_position", dir);
}

void AIManager::_move(double delta)
{
  _rotate_character(delta);
  _blend_patrol_states(delta);
  lookat_player_component->set_look_status(true);
  m_LowerBodyStateMachine->travel("Walk");
  // nav_agent_3d->set_target_position(m_AIStateCtxInst.AIMovePosition);

  // float toPlayerDist = m_AIStateCtxInst.ToPlayerDistance;

  Vector3 horizPos = Vector3(ai_character_component->get_global_position().x, 0.0f, ai_character_component->get_global_position().z);
  // Vector3 movePos = Vector3(m_AIStateCtxInst.AIMovePosition.x, 0.0f, m_AIStateCtxInst.AIMovePosition.z);

  // if(horizPos.is_equal_approx(movePos))
  {
  }

  // if(m_AIStateCtxInst.WantsToShoot)
  {
    _activate_shoot_state(delta);
  }
  // else if(toPlayerDist >= m_AIBehaviourProps->get_playerDistToTriggerChase())
  {
    // m_AIStateCtxInst.TargetForShootReached = false;
    _deactivate_shoot_state(delta);
  }
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
  m_LowerBodyStateMachine->travel("Walk");

  // nav_agent_3d->set_target_position(m_AIStateCtxInst.LastPlayerPosBeforePatrol);

  if(nav_agent_3d->is_navigation_finished())
  {
    // m_AIStateCtxInst.IsNavigationFinished = true;
  }

}

void AIManager::_activate_shoot_state(double delta)
{
  if(m_QueryTimer <= 0.0f)
  {
    env_query3d->request_query();
    m_QueryTimer = m_AIBehaviourProps->get_enemyCombatNewPosQueryTime();
  }

  m_QueryTimer -= delta;
  // m_AIStateCtxInst.WantsToShoot = true;

  anim_tree->set("parameters/UpperBodyBlend/blend_amount", 1.0f);
  m_UpperBodyStateMachine->travel("Shoot");
}

void AIManager::_deactivate_shoot_state(double delta)
{
  // m_AIStateCtxInst.WantsToShoot = false;
  m_UpperBodyStateMachine->travel("Idle");
}