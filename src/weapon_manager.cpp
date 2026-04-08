#include "weapon_manager.h"

WeaponManager::WeaponManager()
{
}

void WeaponManager::_bind_methods()
{
  GD_BIND_PROPERTY(WeaponManager, weaponList, Variant::ARRAY);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_bob_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ADD_SIGNAL(MethodInfo("weapon_equip"));
  ADD_SIGNAL(MethodInfo("weapon_unequip", PropertyInfo(Variant::STRING, "nextWeaponName")));
  ADD_SIGNAL(MethodInfo("weapon_shoot"));
  ADD_SIGNAL(MethodInfo("weapon_reload"));
  ADD_SIGNAL(MethodInfo("weapon_switch"));
  ADD_SIGNAL(MethodInfo("weapon_change", PropertyInfo(Variant::STRING, "nextWeaponName")));

  ADD_SIGNAL(MethodInfo("weapon_equip_finished"));
  ADD_SIGNAL(MethodInfo("weapon_unequip_finished"));
}

void WeaponManager::_ready()
{
  weapon_component->set_current_weapon(weaponList[0]);
}

void WeaponManager::_idle_weapon_sway(double delta)
{
  // m_IdleWeaponBobTime += delta * 0.5f;
  //
  // float x_bob = Math::cos(m_IdleWeaponBobTime * m_IdleWeaponBobFreq * 0.5f) * m_IdleWeaponBobAmp;
  // float y_bob = Math::sin(m_IdleWeaponBobTime * m_IdleWeaponBobFreq) * m_IdleWeaponBobAmp;
  //
  // Vector3 currentPos = m_HoldPointNode->get_position();
  // Vector3 newPos = Vector3(
  //   Utils::exp_decay(currentPos.x, x_bob, m_IdleWeaponBobSmoothVal, (float)delta),
  //   Utils::exp_decay(currentPos.y, y_bob, m_IdleWeaponBobSmoothVal, (float)delta), 
  //   0.0f
  // );
  //
  // if(!m_HoldPointNode->get_transform().is_finite())
  // {
  //   print_error("Idle weapon sway: Transform is infinite!");
  //   GENERATE_TRAP();
  // }
  // m_HoldPointNode->set_position(newPos);

}

void WeaponManager::_reset_weapon_sway(double delta)
{
  // Vector3 m_HoldPointPos = m_HoldPointNode->get_position();
  // m_HoldPointPos.x = Math::lerp(m_HoldPointNode->get_position().x, 0.0f, m_WeaponSwayResetValue * (float)delta);
  // m_HoldPointPos.y = Math::lerp(m_HoldPointNode->get_position().y, 0.0f, m_WeaponSwayResetValue * (float)delta);
  // if(!m_HoldPointNode->get_transform().is_finite())
  // {
  //   print_error("Reset Weapon Sway: Transform is infinite!");
  //   GENERATE_TRAP();
  // }
  // m_HoldPointNode->set_position(m_HoldPointPos);
}

void WeaponManager::_weapon_sway(Vector2 sway_vector)
{
  // Vector3 m_HoldPointPos = m_HoldPointNode->get_position();
  // m_HoldPointPos.x -= sway_vector.x * m_WeaponSwayMult * m_PlayerInst->get_physics_process_delta_time();
  // m_HoldPointPos.y += sway_vector.y * m_WeaponSwayMult * m_PlayerInst->get_physics_process_delta_time();
  //
  // if(!m_HoldPointNode->get_transform().is_finite())
  // {
  //   print_error("Weapon Sway: Transform is infinite!");
  //   GENERATE_TRAP();
  // }
  // m_HoldPointNode->set_position(m_HoldPointPos);
}

// void WeaponManager::set_weapon_state(WeaponStates state, const WeaponAdditionalData& additionalData) 
// { 
   // if(m_CurrentWeaponState != WeaponStates::NONE)
   // {
   //   m_CurrentWeaponState = state;
   // }
   //
   // if(additionalData.index != -1)
   // {
   //   m_WeaponIndex = additionalData.index;
   // }
   //
   // if(additionalData.currentWeapon != nullptr)
   // {
   //   m_CurrentWeapon = additionalData.currentWeapon;
   // }
   //
   // if(additionalData.nextWeaponName != "")
   // {
   //   m_NextWeaponName = additionalData.nextWeaponName;
   // }
 // }


void WeaponManager::_physics_process(double delta)
{
  if(weapon_bob_component)
    weapon_bob_component->weapon_bob(delta);
}

WeaponManager::~WeaponManager()
{
}
