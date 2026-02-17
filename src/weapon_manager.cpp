#include "weapon_manager.h"
#include "player_state_machine.h"

WeaponManager::WeaponManager()
{
  m_IsEquipped = false;
}

void WeaponManager::_bind_methods()
{
  GD_BIND_PROPERTY(WeaponManager, weaponList, Variant::ARRAY);

  ADD_GROUP("Weapon Bob Properties", "");
  GD_BIND_PROPERTY(WeaponManager, weapon_bob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(WeaponManager, weapon_bob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(WeaponManager, idle_weapon_bob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(WeaponManager, idle_weapon_bob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(WeaponManager, idle_weapon_bob_decay, Variant::FLOAT);
  GD_BIND_PROPERTY(WeaponManager, weapon_bob_decay, Variant::FLOAT);
}

void WeaponManager::_ready()
{
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();

  m_WeaponAnimPlayer = get_node<AnimationPlayer>(NodePath("WeaponAnimPlayer"));
  m_LoadScene = ResourceLoader::get_singleton()->load("res://assets/decals/bullet_decal.tscn");

  _init_weapon();
}

void WeaponManager::_unhandled_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("unequip_weapon") && m_IsEquipped)
  {
    m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponUnequipAnimName());
    m_IsEquipped = !m_IsEquipped;
  }
}

void WeaponManager::_init_weapon()
{
  m_CurrentWeapon = weaponList[0];

  if(m_WeaponAnimPlayer) {
    m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName());
    m_IsEquipped = !m_IsEquipped;
  } else {
    print_error("Can't play equip animation!!");
  }

}

void WeaponManager::_weapon_bob(double delta)
{
  bool onFloor = m_PlayerInst->is_on_floor(); // so that bobbing doesn't occur during airborne states

  float velocity = m_PlayerInst->get_velocity().length();
  m_WeaponBobTime += delta * velocity * onFloor;

  float x_bob = Math::cos(m_WeaponBobTime * weapon_bob_freq * 0.5f) * weapon_bob_amp;
  float y_bob = Math::sin(m_WeaponBobTime * weapon_bob_freq) * weapon_bob_amp;

  Vector3 currentPos = m_PlayerInst->get_rig_hold_point()->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, weapon_bob_decay, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, weapon_bob_decay, (float)delta), 
    0.0f
  );

  m_PlayerInst->get_rig_hold_point()->set_position(newPos);
}

void WeaponManager::_shoot()
{
  if(m_WeaponAnimPlayer)
  {
    m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName());
  }

  // This is a mess!
  PhysicsDirectSpaceState3D* space_state = m_PlayerInst->get_player_camera()->get_world_3d()->get_direct_space_state();
  Vector2 screen_center = m_PlayerInst->get_player_camera()->get_viewport()->get_visible_rect().get_size() / 2.0f;

  Vector3 ray_start = m_PlayerInst->get_player_camera()->project_ray_origin(screen_center);
  Vector3 ray_end = ray_start + m_PlayerInst->get_player_camera()->project_ray_normal(screen_center) * 1000.0f;

  Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(ray_start, ray_end);
  query->set_collide_with_bodies(true);
  Dictionary result = space_state->intersect_ray(query);

  if(!result.is_empty())
  {
    Node* instance = m_LoadScene->instantiate();
    Decal* bulletDecal = Object::cast_to<Decal>(instance);
    get_tree()->get_root()->add_child(bulletDecal);
    bulletDecal->set_global_position(result["position"]);
    bulletDecal->look_at(bulletDecal->get_global_transform().origin + result["normal"], Vector3(0.0f, 1.0f, 0.0f));
    bulletDecal->rotate_object_local(Vector3(1.0f, 0.0f, 0.0f), 90.0f);
  }
}

void WeaponManager::_physics_process(double delta)
{
  // TODO: THIS IS PURE ASS. Need a better way to manage states
  if(m_StateMachineInst->get_current_state() == m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::SPRINT)
    || m_StateMachineInst->get_current_state() == m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::CROUCH))
    _weapon_bob(delta);

  if(m_StateMachineInst->get_current_state() == m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::IDLE))
  {
    m_IdleWeaponBobTime += delta * 0.5f;

    float x_bob = Math::cos(m_IdleWeaponBobTime * idle_weapon_bob_freq * 0.5f) * idle_weapon_bob_amp;
    float y_bob = Math::sin(m_IdleWeaponBobTime * idle_weapon_bob_freq) * idle_weapon_bob_amp;

    Vector3 currentPos = m_PlayerInst->get_rig_hold_point()->get_position();
    Vector3 newPos = Vector3(
      Utils::exp_decay(currentPos.x, x_bob, idle_weapon_bob_decay, (float)delta),
      Utils::exp_decay(currentPos.y, y_bob, idle_weapon_bob_decay, (float)delta), 
      0.0f
    );

    m_PlayerInst->get_rig_hold_point()->set_position(newPos);
  }
  
  if(Input::get_singleton()->is_action_just_pressed("fire"))
  {
    _shoot();
  }
}

WeaponManager::~WeaponManager()
{
}
