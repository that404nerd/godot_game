#include "weapon_effects_components.h"

void WeaponBobComponent::_init_data(Node3D* hold_point_node, MovementStateMachine* movementStateMachine, CharacterComponent* characterComponent, WeaponComponent* weaponComponent)
{
  if (!characterComponent) {
    print_error("Character component is null!");
    return;
  }

  if (!weaponComponent) {
    print_error("Weapon component is null!");
    return;
  }

  m_CharacterBody = characterComponent;
  m_MovementStateMachine = movementStateMachine;
  m_CurrentWeapon = weaponComponent->get_current_weapon_data();

  if (!m_CurrentWeapon.is_valid()) {
    print_error("Current weapon is not valid!");
    return;
  }

  m_HoldPointNode = hold_point_node;

  m_WeaponBobFreq = m_CurrentWeapon->get_weapon_bob_freq();
  m_WeaponBobAmp = m_CurrentWeapon->get_weapon_bob_amp();
  m_WeaponBobSmoothVal = m_CurrentWeapon->get_weapon_bob_smooth_val();
}

void WeaponBobComponent::_update_bob_data(Ref<Weapon> currentWeapon)
{
  m_WeaponBobFreq = currentWeapon->get_weapon_bob_freq();
  m_WeaponBobAmp = currentWeapon->get_weapon_bob_amp();
  m_WeaponBobSmoothVal = currentWeapon->get_weapon_bob_smooth_val();

  m_BobVector = Vector3(0.0f, 0.0f, 0.0f);
}

void WeaponBobComponent::weapon_bob(double delta)
{
  if (!m_CharacterBody) return;
  
  m_CharacterVel = m_CharacterBody->get_velocity();

  bool onFloor = m_CharacterBody->is_on_floor();
  float velocityMag = m_CharacterVel.length();

  if(m_MovementStateMachine->get_current_state() != static_cast<int>(MovementStates::SPRINT) &&
     m_MovementStateMachine->get_current_state() != static_cast<int>(MovementStates::CROUCH))
  {
    Vector3 currentHoldPoint = m_HoldPointNode->get_position();
    m_BobOffset = m_BobOffset.lerp(currentHoldPoint, (float)delta);
    return;
  }

  m_WeaponBobTime += delta * velocityMag * onFloor;

  if(velocityMag > 0.1f)
  {
    m_BobVector.x = Math::cos(m_WeaponBobTime * m_WeaponBobFreq * 0.618f) * m_WeaponBobAmp;
    m_BobVector.y = Math::sin(m_WeaponBobTime * m_WeaponBobFreq) * m_WeaponBobAmp;
  }

  m_BobOffset.x = Utils::exp_decay(m_BobOffset.x, m_BobVector.x, m_WeaponBobSmoothVal, (float)delta);
  m_BobOffset.y = Utils::exp_decay(m_BobOffset.y, m_BobVector.y, m_WeaponBobSmoothVal, (float)delta);
  m_BobOffset.z = 0.0f;
}

void WeaponSwayComponent::_init_data(MovementStateMachine* movementStateMachine, CharacterComponent* characterComponent, WeaponComponent* weaponComponent)
{
  if(!characterComponent) {
    print_error("Character component is null!");
    return;
  }

  if(!weaponComponent) {
    print_error("Weapon component is null!");
    return;
  }

  m_MovementStateMachine = movementStateMachine;

  m_CharacterBody = characterComponent;
  m_CurrentWeapon = weaponComponent->get_current_weapon_data();

  if(!m_CurrentWeapon.is_valid()) {
    print_error("Current weapon is not valid!");
    return;
  }

  m_IdleWeaponBobFreq = m_CurrentWeapon->get_idle_weapon_bob_freq();
  m_IdleWeaponBobAmp = m_CurrentWeapon->get_idle_weapon_bob_amp();
  m_IdleWeaponBobSmoothVal = m_CurrentWeapon->get_idle_weapon_bob_smooth_val();
  
  m_WeaponSpringAngFreq = m_CurrentWeapon->get_weaponSwayAngularFreq();
  m_WeaponDampedSpringRatio = m_CurrentWeapon->get_weaponSwayDampingRatio();
}

void WeaponSwayComponent::_update_sway_data(Ref<Weapon> currentWeapon)
{
  m_IdleWeaponBobFreq = currentWeapon->get_idle_weapon_bob_freq();
  m_IdleWeaponBobAmp = currentWeapon->get_idle_weapon_bob_amp();
  m_IdleWeaponBobSmoothVal = currentWeapon->get_idle_weapon_bob_smooth_val();

  m_IdleSwayVector = Vector3(0.0f, 0.0f, 0.0f);
}

void WeaponSwayComponent::weapon_idle_sway(double delta)
{
  if (!m_CharacterBody) return;

  m_CharacterVel = m_CharacterBody->get_velocity();

  if(m_CharacterVel.length() > 0.1f || m_MovementStateMachine->get_current_state() == static_cast<int>(MovementStates::SLIDE))
  {
    m_IdleSwayOffset = m_IdleSwayOffset.lerp(Vector3(0.0f, 0.0f, 0.0f), (float)delta);
    return;
  }

  m_IdleWeaponBobTime += delta;

  m_IdleSwayVector.x = Math::sin(m_IdleWeaponBobTime * m_IdleWeaponBobFreq) * m_IdleWeaponBobAmp;
  m_IdleSwayVector.y = Math::sin(m_IdleWeaponBobTime * m_IdleWeaponBobFreq * 0.618f) * m_IdleWeaponBobAmp;

  m_IdleSwayOffset.x = Utils::exp_decay(m_IdleSwayOffset.x, m_IdleSwayVector.x, m_IdleWeaponBobSmoothVal, (float)delta);
  m_IdleSwayOffset.y = Utils::exp_decay(m_IdleSwayOffset.y, m_IdleSwayVector.y, m_IdleWeaponBobSmoothVal, (float)delta);
}

void WeaponSwayComponent::weapon_sway(double delta, Vector3& sway_vel)
{
  Vector3 equilibriumPos {};

  m_DampedSpring.CalcDampedSpringMotionParams(
    (float)delta,
    m_WeaponSpringAngFreq,
    m_WeaponDampedSpringRatio);

  m_DampedSpring.UpdateDampedSpringMotion(m_SwayOffset, sway_vel, equilibriumPos);
}

void WeaponEffects::_init_data(MovementStateMachine* movementStateMachine, Node3D* holdPointNode,
                               CharacterComponent* characterComponent,
                               WeaponComponent* weaponComponent)
{
  m_HoldPointNode = holdPointNode;
  m_BasePos = m_HoldPointNode->get_position();
  m_BaseRot = m_HoldPointNode->get_rotation();

  m_WeaponBobComponent._init_data(holdPointNode, movementStateMachine, characterComponent, weaponComponent);
  m_WeaponSwayComponent._init_data(movementStateMachine, characterComponent, weaponComponent);
}

void WeaponEffects::_update_data(Ref<Weapon> currentWeapon)
{
  m_WeaponBobComponent._update_bob_data(currentWeapon);
  m_WeaponSwayComponent._update_sway_data(currentWeapon);
}

void WeaponEffects::_update(double delta, Vector3& sway_vel)
{
  m_WeaponSwayComponent.weapon_idle_sway(delta);
  m_WeaponSwayComponent.weapon_sway(delta, sway_vel);
  m_WeaponBobComponent.weapon_bob(delta);

  if (!m_HoldPointNode) return;

  m_HoldPointNode->set_position(
    m_BasePos +
    m_WeaponSwayComponent.get_sway_offset() +
    m_WeaponBobComponent.get_weapon_bob_offset());

  Vector3 holdPointRot = Vector3(m_BaseRot.x + m_WeaponSwayComponent.get_idle_sway_offset().x, m_BaseRot.y + m_WeaponSwayComponent.get_idle_sway_offset().y, m_HoldPointNode->get_rotation().z);
  m_HoldPointNode->set_rotation(holdPointRot);
}
