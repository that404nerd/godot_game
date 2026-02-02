#include "weapon_manager.h"

WeaponManager::WeaponManager()
{

}

void WeaponManager::_bind_methods()
{
  GD_BIND_PROPERTY(WeaponManager, weaponList, Variant::ARRAY);

  ADD_GROUP("Headbob Properties", "");
  GD_BIND_PROPERTY(WeaponManager, headbob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(WeaponManager, headbob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(WeaponManager, headbob_delta_translate, Variant::FLOAT);
}

void WeaponManager::_ready()
{
  m_WeaponHoldPoint = get_node<Node3D>(NodePath("../CameraController/PlayerHead/Camera3D/WeaponViewportContainer/WeaponViewport/WeaponCamera/HoldPoint"));
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
  _init_weapon();
}

void WeaponManager::_input(const Ref<InputEvent>& event)
{
}

void WeaponManager::_init_weapon()
{
  m_CurrentWeapon = weaponList[0];

  m_WeaponNode = Object::cast_to<Node3D>(m_CurrentWeapon->get_weaponScene()->instantiate());
  m_WeaponHoldPoint->add_child(m_WeaponNode);
    

  //   if(m_WeaponAnimPlayer) {
  //       m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName());
  //   } else {
  //     print_error("Can't play equip animation!!");
  //   }
}

void WeaponManager::_weapon_sway(double delta)
{
  bool onFloor = m_PlayerInst->is_on_floor(); // so that bobbing doesn't occur during airborne states

  float velocity = m_PlayerInst->get_velocity().length();
  m_HeadbobTime += delta * velocity * onFloor;

  float x_bob = Math::cos(m_HeadbobTime * headbob_freq * 0.5f) * headbob_amp;
  float y_bob = Math::sin(m_HeadbobTime * headbob_freq) * headbob_amp;

  Vector3 currentPos = m_PlayerInst->get_player_head()->get_position();
  Vector3 newPos = Vector3(
    Math::lerp(currentPos.x, x_bob, (float)delta * headbob_delta_translate),
    Math::lerp(currentPos.y, y_bob, (float)delta * headbob_delta_translate), 
    0.0f
  );

  m_PlayerInst->get_player_head()->set_position(newPos);
}

void WeaponManager::_physics_process(double delta)
{
  // TODO: Maybe switch to Bits instead of enums
  if(m_StateMachineInst->get_current_state() == m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::SPRINT) ||
      m_StateMachineInst->get_current_state() == m_StateMachineInst->GetCurrentState(PlayerStateMachine::StateNames::CROUCH)) {
    _weapon_sway(delta);
  }
}

WeaponManager::~WeaponManager()
{
}
