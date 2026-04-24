#include "weapon_effects_components.h"
#include "../weapon_state_machine.h" 

void WeaponSwayComponent::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponSwayComponent, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponSwayComponent, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponSwayComponent, hold_point_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}


void WeaponSwayComponent::_ready()
{
  if(character_component)
    m_CharacterBody = character_component->get_character_body();

  _init_data();
}

void WeaponSwayComponent::_init_data()
{
  if(weapon_component)
    m_CurrentWeapon = weapon_component->get_current_weapon_data();

  m_WeaponSwayResetValue = m_CurrentWeapon->get_weapon_sway_reset();
  m_IdleWeaponBobFreq = m_CurrentWeapon->get_idle_weapon_bob_freq();
  m_IdleWeaponBobAmp = m_CurrentWeapon->get_idle_weapon_bob_amp();
  m_IdleWeaponBobSmoothVal = m_CurrentWeapon->get_idle_weapon_bob_smooth_val();

  m_WeaponSpringAngFreq = m_CurrentWeapon->get_angularFreq();
  m_WeaponSpringDampingRatio = m_CurrentWeapon->get_dampingRatio();

}

void WeaponSwayComponent::reset_weapon_sway(double delta)
{
  m_HoldPointPos = hold_point_node->get_position();
  m_HoldPointPos.x = Math::lerp(m_HoldPointPos.x, 0.0f, m_WeaponSwayResetValue * (float)delta);
  m_HoldPointPos.y = Math::lerp(m_HoldPointPos.y, 0.0f, m_WeaponSwayResetValue * (float)delta);

  hold_point_node->set_position(m_HoldPointPos);
}

void WeaponSwayComponent::_process(double delta) 
{
  // TODO: Fix this!
//   m_WeaponStateID = weapon_state_machine->get_current_state();
//   if(m_WeaponStateID == static_cast<int8_t>(WeaponStates::WEAPON_SWITCH))
//   {
    _init_data();
//   }
}

void WeaponSwayComponent::weapon_idle_sway(double delta)
{
  m_IdleWeaponBobTime += delta * 0.5f;

  float x_bob = Math::cos(m_IdleWeaponBobTime * m_IdleWeaponBobFreq * 0.5f) * m_IdleWeaponBobAmp;
  float y_bob = Math::sin(m_IdleWeaponBobTime * m_IdleWeaponBobFreq) * m_IdleWeaponBobAmp;

  Vector3 currentPos = hold_point_node->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, m_IdleWeaponBobSmoothVal, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, m_IdleWeaponBobSmoothVal, (float)delta), 
    0.0f
  );

  hold_point_node->set_position(newPos);
}

void WeaponSwayComponent::weapon_sway(double delta, Vector3& sway_vel)
{
  if(!m_CharacterBody || !hold_point_node) return;
  m_HoldPointPos = hold_point_node->get_position();

  Vector3 equilibriumPos = Vector3(0, 0, 0);
  Vector3 currentPos = hold_point_node->get_position();

  Utils::CalcDampedSpringMotionParams(
      m_SwayParams, 
      (float)delta, 
      m_WeaponSpringAngFreq,
      m_WeaponSpringDampingRatio
  );

  Utils::UpdateDampedSpringMotion(currentPos, sway_vel, equilibriumPos, m_SwayParams);

  currentPos.x = Math::clamp(currentPos.x, Math::lerp(-0.049f, -0.05f, (float)delta), Math::lerp(0.049f, 0.05f, (float)delta));
  currentPos.y = Math::clamp(currentPos.y, Math::lerp(-0.049f, -0.05f, (float)delta), Math::lerp(0.049f, 0.05f, (float)delta));

  hold_point_node->set_position(currentPos);
}