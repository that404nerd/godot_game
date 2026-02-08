#include "weapon_manager.h"
#include "godot_cpp/classes/physics_direct_space_state3d.hpp"
#include "godot_cpp/classes/physics_ray_query_parameters3d.hpp"
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
}

void WeaponManager::_ready()
{
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();

  m_WeaponAnimPlayer = get_node<AnimationPlayer>(NodePath("WeaponAnimPlayer"));

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
    Math::lerp(currentPos.x, x_bob, (float)delta),
    Math::lerp(currentPos.y, y_bob, (float)delta), 
    0.0f
  );

  m_PlayerInst->get_rig_hold_point()->set_position(newPos);
}

void WeaponManager::_shoot()
{
  PhysicsDirectSpaceState3D* space_state = m_PlayerInst->get_player_camera()->get_world_3d()->get_direct_space_state();
  Vector3 ray_start = m_PlayerInst->get_player_camera()->get_global_position();

  Basis cam_basis = m_PlayerInst->get_player_camera()->get_global_transform().basis;

  // Pull out the "Forward" direction (Negative Z)
  Vector3 forward = -cam_basis.get_column(2);
  Vector3 ray_end = ray_start + forward.normalized() * 1000.0f;
  Ref<PhysicsRayQueryParameters3D> params = PhysicsRayQueryParameters3D::create(ray_start, ray_end);
  params->set_collide_with_bodies(true);
  Dictionary result = space_state->intersect_ray(params);

  print_line(result);
}

void WeaponManager::_physics_process(double delta)
{

  // THIS IS PURE ASS. Need a better way to manage states
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
      Math::lerp(currentPos.x, x_bob, (float)delta),
      Math::lerp(currentPos.y, y_bob, (float)delta), 
      0.0f
    );

    m_PlayerInst->get_rig_hold_point()->set_position(newPos);
  
  }
  
  if(Input::get_singleton()->is_action_just_pressed("fire"))
    _shoot();
}

WeaponManager::~WeaponManager()
{
}
