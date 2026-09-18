#include "ai_manager.h"
#include "../components/ai/ai_character_component.h"

void AIManager::_init()
{
  m_LowerBodyStateMachine = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/LowerBodyStateMachine/playback"));
  m_UpperBodyStateMachine = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/UpperBodyStateMachine/playback"));

  m_AIBehaviourProps = ai_character_component->get_ai_behaviour_props();
  m_BlackboardInst = ai_character_component->get_bt_player_inst()->get_blackboard();

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
}

void AIManager::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_query_finished", "queryResult"), &AIManager::_on_query_finished);
  ClassDB::bind_method(D_METHOD("_idle", "delta"), &AIManager::_idle);

  GD_BIND_CUSTOM_PROPERTY(AIManager, AICharacterComponent, ai_character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, InputCommandSystem, input_cmd_system, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, NavigationAgent3D, nav_agent_3d, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, EnvironmentQuery3D, env_query3d, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, AnimationTree, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, LookAtPlayerComponent, lookat_player_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, VisionComponent, ai_vision_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
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

  m_BlackboardInst->set_var("AIVelocity", ai_character_component->get_velocity());
  m_BlackboardInst->set_var("ToPlayerDistance", (m_Target->get_global_position() - ai_character_component->get_global_position()).length());
  m_BlackboardInst->set_var("NextNavigationPoint", nav_agent_3d->get_next_path_position());
  
  m_BlackboardInst->set_var("ToPlayerDirection", m_Target->get_global_position() - ai_character_component->get_global_position());
  m_BlackboardInst->set_var("AIDirection", (nav_agent_3d->get_next_path_position() - ai_character_component->get_global_position()).normalized());
  
  if(ai_vision_component)
    m_BlackboardInst->set_var("CanSeePlayer", ai_vision_component->can_see_player());
  
  ai_character_component->set_wish_dir(m_BlackboardInst->get_var("AIDirection"));
  nav_agent_3d->set_velocity(ai_character_component->get_velocity());

  if(lookat_player_component)
    lookat_player_component->_update(delta);

}

void AIManager::_physics_update(double delta)
{
  if(ai_vision_component)
    ai_vision_component->_physics_update(delta);
}

void AIManager::_rotate_character(double delta)
{
  Vector3 direction = m_BlackboardInst->get_var("ToPlayerDirection");
  if(direction.length() > 0.01f)
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
  input_cmd_system->command(MovementInputCommands::IDLE);
  lookat_player_component->set_look_status(false);
  m_BlackboardInst->set("IsNavigationFinished", true);

  m_LowerBodyStateMachine->travel("Idle");
}

void AIManager::_blend_chase_states(double delta)
{
  Vector3 aiDir = m_BlackboardInst->get_var("AIDirection", Vector3(0.0f, 0.0f, 0.0f));
  Vector2 dir = Vector2(aiDir.x, aiDir.z).normalized();
  anim_tree->set("parameters/LowerBodyStateMachine/Run/blend_position", dir);
}

BT::Status AIManager::_chase(double delta, bool shouldRotate)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return BT::FAILURE;
  }

  input_cmd_system->command(MovementInputCommands::SPRINT);

  _blend_chase_states(delta);

  if(shouldRotate)
    _rotate_character(delta);
  lookat_player_component->set_look_status(shouldRotate);

  m_LowerBodyStateMachine->travel("Run");

  nav_agent_3d->set_target_desired_distance(m_AIBehaviourProps->get_enemyDistBetweenPlayer());
  nav_agent_3d->set_target_position(m_Target->get_global_position());

  if(nav_agent_3d->is_navigation_finished())
  {
    m_BlackboardInst->set_var("IsNavigationFinished", true);
    return BT::SUCCESS;
  }

  return BT::RUNNING;
}

void AIManager::_blend_patrol_states(double delta)
{
  Vector3 aiDir = m_BlackboardInst->get_var("AIDirection", Vector3(0.0f, 0.0f, 0.0f));
  Vector2 dir = Vector2(aiDir.x, aiDir.z).normalized();
  anim_tree->set("parameters/Patrol/blend_position", dir);
}

BT::Status AIManager::_patrol(double delta, bool shouldRotate)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return BT::FAILURE;
  }
  
  input_cmd_system->command(MovementInputCommands::WALK);

  if(shouldRotate)
    _rotate_character(delta);
  lookat_player_component->set_look_status(shouldRotate);

  _blend_patrol_states(delta);
  m_LowerBodyStateMachine->travel("Walk");

  nav_agent_3d->set_target_desired_distance(m_AIBehaviourProps->get_enemyDistBetweenPlayer());
  nav_agent_3d->set_target_position(m_Target->get_global_position());

  if(nav_agent_3d->is_navigation_finished())
  {
    m_BlackboardInst->set_var("IsNavigationFinished", true);
    return BT::SUCCESS;
  }

  return BT::RUNNING;

}

BT::Status AIManager::_shoot(double delta)
{
  if(ai_vision_component->can_see_player() && m_Target)
  {
    if(m_QueryTimer <= 0.0f)
    {
      env_query3d->request_query();
      m_QueryTimer = m_AIBehaviourProps->get_enemyCombatNewPosQueryTime();
    }
  
    m_QueryTimer -= delta;
  
    _rotate_character(delta);
    lookat_player_component->set_look_status(true);
  
    anim_tree->set("parameters/UpperBodyBlend/blend_amount", 1.0f);
    m_UpperBodyStateMachine->travel("Shoot");
    
    return BT::RUNNING;
  }

  m_UpperBodyStateMachine->travel("Idle");
  return BT::FAILURE;
}