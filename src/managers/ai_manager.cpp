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

  EventBus::get_singleton()->connect("ammo_finished", Callable(this, "_on_ammo_finished"));
  EventBus::get_singleton()->connect("death", Callable(this, "_on_health_finished"));
}

void AIManager::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_query_finished", "queryResult"), &AIManager::_on_query_finished);
  ClassDB::bind_method(D_METHOD("_idle", "delta"), &AIManager::_idle);
  ClassDB::bind_method(D_METHOD("_shoot", "delta"), &AIManager::_shoot);
  ClassDB::bind_method(D_METHOD("_shoot", "delta"), &AIManager::_shoot);
  ClassDB::bind_method(D_METHOD("_on_health_finished"), &AIManager::_on_health_finished);
  ClassDB::bind_method(D_METHOD("_death"), &AIManager::_death);

  ClassDB::bind_method(D_METHOD("_on_ammo_finished"), &AIManager::_on_ammo_finished);
  ClassDB::bind_method(D_METHOD("_reload"), &AIManager::_reload);

  GD_BIND_CUSTOM_PROPERTY(AIManager, AICharacterComponent, ai_character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, InputCommandSystem, input_cmd_system, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, NavigationAgent3D, nav_agent_3d, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, EnvironmentQuery3D, env_query3d, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, AnimationTree, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, VisionComponent, ai_vision_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ADD_GROUP("IK Nodes", "");
  GD_BIND_CUSTOM_PROPERTY(AIManager, CopyTransformModifier3D, rArmCopyModifier, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, CopyTransformModifier3D, rHandCopyModifier, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, CopyTransformModifier3D, spineCopyModifer, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  GD_BIND_CUSTOM_PROPERTY(AIManager, TwoBoneIK3D, lHandIK, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, CopyTransformModifier3D, lHandCopyModifier, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AIManager, CopyTransformModifier3D, lHandThumbCopyModifier, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void AIManager::_on_query_finished(QueryResult3D* queryResult)
{
  Vector3 best_pos = queryResult->get_highest_score_position();

  if(nav_agent_3d->is_navigation_finished())
  {
    m_BlackboardInst->set_var("AIMovePosition", best_pos);
  }
}

void AIManager::_update(double delta)
{
  if(ai_vision_component)
    ai_vision_component->_update(delta);
}

void AIManager::_physics_update(double delta)
{
  MoveCommand& move_cmd = input_cmd_system->get_move_command();

  Vector3 target_pos = m_Target->get_global_position();
  Vector3 ai_pos = ai_character_component->get_global_position();
  
  if(ai_vision_component)
  {
    ai_vision_component->_physics_update(delta);
    m_BlackboardInst->set_var("CanSeePlayer", ai_vision_component->can_see_player());
  }

  if(!nav_agent_3d->is_navigation_finished())
  {
    m_NextPathPos = nav_agent_3d->get_next_path_position();
    m_BlackboardInst->set_var("NextNavigationPoint", m_NextPathPos);

    move_cmd.CharacterWishDir = (m_NextPathPos - ai_character_component->get_global_position()).normalized(),
    m_BlackboardInst->set_var("AIDirection", move_cmd.CharacterWishDir);
  }

  m_BlackboardInst->set_var("AIVelocity", move_cmd.CharacterVelocity);
  m_BlackboardInst->set_var("ToPlayerDirection", target_pos - ai_pos);
  m_BlackboardInst->set_var("ToPlayerDistance", (target_pos - ai_pos).length());

  ai_character_component->set_wish_dir(move_cmd.CharacterWishDir);
  nav_agent_3d->set_velocity(move_cmd.CharacterVelocity);
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

void AIManager::_enable_shootIK(bool enable)
{
  if(enable)
  {
    rArmCopyModifier->set("settings/0/amount", 1.0f);
    rHandCopyModifier->set("settings/0/amount", 1.0f);
    spineCopyModifer->set("settings/0/amount", 1.0f);
  } else {
    rArmCopyModifier->set("settings/0/amount", 0.0f);
    rHandCopyModifier->set("settings/0/amount", 0.0f);
    spineCopyModifer->set("settings/0/amount", 0.0f);
  }
}

void AIManager::_on_ammo_finished()
{
  m_BlackboardInst->set_var("IsAmmoEmpty", true);
}

void AIManager::_blend_chase_states(double delta)
{
  Vector3 aiDir = m_BlackboardInst->get_var("AIDirection", Vector3(0.0f, 0.0f, 0.0f));
  Vector2 dir = Vector2(aiDir.x, aiDir.z).normalized();
  anim_tree->set("parameters/LowerBodyStateMachine/Run/blend_position", dir);
}

void AIManager::_blend_patrol_states(double delta)
{
  Vector3 aiDir = m_BlackboardInst->get_var("AIDirection", Vector3(0.0f, 0.0f, 0.0f));
  Vector2 dir = Vector2(aiDir.x, aiDir.z).normalized();
  anim_tree->set("parameters/Patrol/blend_position", dir);
}

void AIManager::_idle(double delta)
{
  MoveCommand& move_cmd = input_cmd_system->get_move_command();

  move_cmd = {
    .IsOnFloor = ai_character_component->is_on_floor(),
    .WantsToIdle = true,
  };

  _enable_shootIK(false);
  m_LowerBodyStateMachine->travel("Idle");
  anim_tree->set("parameters/UpperBodyBlend/blend_amount", 0.0f);
}


BT::Status AIManager::_chase(double delta, bool shouldRotate, bool toPlayer)
{
  if(!m_Target)
  {
    print_error("Player not found to chase!");
    return BT::FAILURE;
  }

  MoveCommand& move_cmd = input_cmd_system->get_move_command();
  Vector3 aiMovePos = m_BlackboardInst->get_var("AIMovePosition", Vector3(0.0f, 0.0f, 0.0f));


  move_cmd = {
    .CharacterVelocity = ai_character_component->get_velocity(),
    .IsOnFloor = ai_character_component->is_on_floor(),
    .WantsToSprint = true,
  };

  if(toPlayer)
  {
    m_BlackboardInst->set_var("AIMovePosition", m_Target->get_global_position());
    nav_agent_3d->set_target_desired_distance(m_AIBehaviourProps->get_enemyDistBetweenPlayer());
  }

  _enable_shootIK(false);
  _blend_chase_states(delta);

  if(shouldRotate)
    _rotate_character(delta);

  m_LowerBodyStateMachine->travel("Run");
  nav_agent_3d->set_target_position(aiMovePos);

  if(nav_agent_3d->is_navigation_finished())
  {
    return BT::SUCCESS;
  }

  return BT::RUNNING;
}


BT::Status AIManager::_patrol(double delta, bool shouldRotate, bool toPlayer)
{
  MoveCommand& move_cmd = input_cmd_system->get_move_command();

  nav_agent_3d->set_target_desired_distance(0.01f);

  Vector3 aiMovePos = m_BlackboardInst->get_var("AIMovePosition", Vector3(0.0f, 0.0f, 0.0f));

  move_cmd = {
    .CharacterVelocity = ai_character_component->get_velocity(),
    .IsOnFloor = ai_character_component->is_on_floor(),
    .WantsToWalk = true,
  };

  _enable_shootIK(false);
  _blend_patrol_states(delta);

  if(shouldRotate)
    _rotate_character(delta);

  m_LowerBodyStateMachine->travel("Walk");
  nav_agent_3d->set_target_position(aiMovePos);

  if(nav_agent_3d->is_navigation_finished())
  {
    return BT::SUCCESS;
  } 

  return BT::RUNNING;
}

BT::Status AIManager::_shoot(double delta)
{
  WeaponCommand& weapon_cmd = input_cmd_system->get_weapon_command();
  
  if(ai_vision_component->can_see_player() && m_Target)
  {
    // FIXME: Should be hold trigger but it works for now
    weapon_cmd = {
      .WantsToPressTrigger = true
    };

    if(nav_agent_3d->is_navigation_finished())
    {
      env_query3d->request_query();
    }

    _rotate_character(delta);
    _enable_shootIK(true);

    anim_tree->set("parameters/UpperBodyBlend/blend_amount", 1.0f);
    
    return BT::SUCCESS;
  }

  return BT::FAILURE;
}

void AIManager::_reload()
{
  WeaponCommand& weapon_cmd = input_cmd_system->get_weapon_command();
  weapon_cmd = {
    .WantsToReloadWeapon = true
  };
}

void AIManager::_on_health_finished()
{
  m_BlackboardInst->set_var("IsHealthZero", true);
}

void AIManager::_death()
{
  MoveCommand& move_cmd = input_cmd_system->get_move_command();
  WeaponCommand& weapon_cmd = input_cmd_system->get_weapon_command();

  // FIXME: The player might die if on the edge of a roof or something and might not have gravity applied (Edge case)
  move_cmd = {
    .IsOnFloor = true,
    .WantsToIdle = true
  };
  weapon_cmd = {};

  lHandIK->set_active(false);
  lHandCopyModifier->set_active(false);
  lHandThumbCopyModifier->set_active(false);

  // NOTE: Make sure that for these transitions, Allow Transition To Self is Disabled!!
  anim_tree->set("parameters/Transition/transition_request", "DeathTransition");
}