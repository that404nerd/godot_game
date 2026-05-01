#include "weapon_effects_components.h"

void WeaponBobComponent::_init_data(CharacterComponent* characterComponent, WeaponComponent* weaponComponent)
{
  if (!characterComponent) {
    print_error("Character component is null!");
    return;
  }

  if (!weaponComponent) {
    print_error("Weapon component is null!");
    return;
  }

  m_CharacterBody = characterComponent->get_character_body();
  m_CurrentWeapon = weaponComponent->get_current_weapon_data();

  if (!m_CurrentWeapon.is_valid()) {
    print_error("Current weapon is not valid!");
    return;
  }

  m_WeaponBobFreq = m_CurrentWeapon->get_weapon_bob_freq();
  m_WeaponBobAmp = m_CurrentWeapon->get_weapon_bob_amp();
  m_WeaponBobSmoothVal = m_CurrentWeapon->get_weapon_bob_smooth_val();
}

void WeaponBobComponent::weapon_bob(double delta)
{
  if (!m_CharacterBody) return;

  m_PlayerVel = m_CharacterBody->get_velocity();
  bool onFloor = m_CharacterBody->is_on_floor();

  float velocity = m_PlayerVel.length();

  m_WeaponBobTime += delta * velocity * onFloor;

  float x_bob = Math::cos(m_WeaponBobTime * m_WeaponBobFreq * 0.5f) * m_WeaponBobAmp;
  float y_bob = Math::sin(m_WeaponBobTime * m_WeaponBobFreq) * m_WeaponBobAmp;

  m_BobOffset.x = Utils::exp_decay(m_BobOffset.x, x_bob, m_WeaponBobSmoothVal, (float)delta);
  m_BobOffset.y = Utils::exp_decay(m_BobOffset.y, y_bob, m_WeaponBobSmoothVal, (float)delta);
  m_BobOffset.z = 0.0f;
}

void WeaponSwayComponent::_init_data(CharacterComponent* characterComponent, WeaponComponent* weaponComponent)
{
  if (!characterComponent) {
    print_error("Character component is null!");
    return;
  }

  if (!weaponComponent) {
    print_error("Weapon component is null!");
    return;
  }

  m_CharacterBody = characterComponent->get_character_body();
  m_CurrentWeapon = weaponComponent->get_current_weapon_data();

  if (!m_CurrentWeapon.is_valid()) {
    print_error("Current weapon is not valid!");
    return;
  }

  m_IdleWeaponBobFreq = m_CurrentWeapon->get_idle_weapon_bob_freq();
  m_IdleWeaponBobAmp = m_CurrentWeapon->get_idle_weapon_bob_amp();
  m_IdleWeaponBobSmoothVal = m_CurrentWeapon->get_idle_weapon_bob_smooth_val();

  m_WeaponSpringAngFreq = m_CurrentWeapon->get_angularFreq();
  m_WeaponSpringDampingRatio = m_CurrentWeapon->get_dampingRatio();
  
  

}

void WeaponSwayComponent::weapon_idle_sway(double delta)
{
  
  if (!m_CharacterBody) return;

  bool isNotInMotion = m_CharacterBody->get_velocity().length() <= 0.1f;
  
  m_IdleWeaponBobTime += delta * 0.5f * isNotInMotion;

  float x_bob = Math::cos(m_IdleWeaponBobTime * m_IdleWeaponBobFreq * 0.5f) * m_IdleWeaponBobAmp;
  float y_bob = Math::sin(m_IdleWeaponBobTime * m_IdleWeaponBobFreq) * m_IdleWeaponBobAmp;

  m_IdleSwayOffset.x = Utils::exp_decay(m_IdleSwayOffset.x, x_bob, m_IdleWeaponBobSmoothVal, (float)delta);
  m_IdleSwayOffset.y = Utils::exp_decay(m_IdleSwayOffset.y, y_bob, m_IdleWeaponBobSmoothVal, (float)delta);
  m_IdleSwayOffset.z = 0.0f;
}

void WeaponSwayComponent::weapon_sway(double delta, Vector3& sway_vel)
{
  Vector3 equilibriumPos {};

  Utils::CalcDampedSpringMotionParams(
      m_SwayParams,
      (float)delta,
      m_WeaponSpringAngFreq,
      m_WeaponSpringDampingRatio);

  Utils::UpdateDampedSpringMotion(
      m_SwayOffset,
      sway_vel,
      equilibriumPos,
      m_SwayParams);

  m_SwayOffset.x = Math::clamp(m_SwayOffset.x, -0.05f, 0.05f);
  m_SwayOffset.y = Math::clamp(m_SwayOffset.y, -0.05f, 0.05f);
}

void WeaponEffects::_init_data(Node3D* holdPointNode,
                               CharacterComponent* characterComponent,
                               WeaponComponent* weaponComponent)
{
  m_HoldPointNode = holdPointNode;

  m_WeaponBobComponent._init_data(characterComponent, weaponComponent);
  m_WeaponSwayComponent._init_data(characterComponent, weaponComponent);
}

void WeaponEffects::_update(double delta, Vector3& sway_vel)
{
  m_WeaponSwayComponent.weapon_idle_sway(delta);
  m_WeaponSwayComponent.weapon_sway(delta, sway_vel);
  m_WeaponBobComponent.weapon_bob(delta);

  if (!m_HoldPointNode) return;

  m_HoldPointNode->set_position(
      m_WeaponSwayComponent.get_idle_offset() +
      m_WeaponSwayComponent.get_sway_offset() +
      m_WeaponBobComponent.get_offset());
}