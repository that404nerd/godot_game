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
}

WeaponManager::~WeaponManager()
{
}
