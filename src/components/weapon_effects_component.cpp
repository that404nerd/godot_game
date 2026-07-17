#include "weapon_effects_components.h"
#include "../weapon_manager.h"

void WeaponBobComponent::_init_data(const WeaponEffectsData& weaponEffectsData)
{
  m_CharacterBody = weaponEffectsData.CharacterCompInst;
  m_MovementManager = weaponEffectsData.MovementManagerInst;
  m_CurrentWeapon = weaponEffectsData.WeaponCompInst->get_current_weapon_data();
  m_HoldPointNode = weaponEffectsData.HoldPointNode;

  if (!weaponEffectsData.CharacterCompInst || !weaponEffectsData.WeaponCompInst) {
    print_error("Character component or Weapon component is null!");
    return;
  }

  if (!m_CurrentWeapon.is_valid()) {
    print_error("Current weapon is not valid!");
    return;
  }

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

  if(!m_MovementManager->IsSprinting() && !m_MovementManager->IsCrouching())
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

void WeaponSwayComponent::_init_data(const WeaponEffectsData& weaponEffectsData)
{
  m_CharacterBody = weaponEffectsData.CharacterCompInst;
  m_MovementManager = weaponEffectsData.MovementManagerInst;
  m_CurrentWeapon = weaponEffectsData.WeaponCompInst->get_current_weapon_data();

  if(!weaponEffectsData.CharacterCompInst || !weaponEffectsData.WeaponCompInst) {
    print_error("Character component is null!");
    return;
  }

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

  if(m_CharacterVel.length() > 0.1f || m_MovementManager->IsSliding())
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


void WeaponJumpEffect::_init_data(const WeaponEffectsData& weaponEffectsData)
{
  m_CharacterBody = weaponEffectsData.CharacterCompInst;
  m_MovementManager = weaponEffectsData.MovementManagerInst;
  m_CurrentWeapon = weaponEffectsData.WeaponCompInst->get_current_weapon_data();
  m_WeaponManager = weaponEffectsData.WeaponManagerInst;
  m_WeaponArmatureNode = m_WeaponManager->get_weapon_armature_node();

  if(!weaponEffectsData.CharacterCompInst || !weaponEffectsData.WeaponCompInst) {
    print_error("Character component is null!");
    return;
  }

  if(!m_CurrentWeapon.is_valid()) {
    print_error("Current weapon is not valid!");
    return;
  }

  m_JumpArmatureRiseAngFreq = m_CurrentWeapon->get_jump_armature_weapon_rise_ang_freq();
  m_JumpArmatureRiseDampingRatio = m_CurrentWeapon->get_jump_armature_weapon_rise_damping_ratio();
  m_JumpArmatureWeaponRotAngFreq = m_CurrentWeapon->get_jump_armature_weapon_rot_ang_freq();
  m_JumpArmatureWeaponRotDampingRatio = m_CurrentWeapon->get_jump_armature_weapon_rot_damping_ratio();

  m_JumpArmatureWeaponPos = m_CurrentWeapon->get_jump_armature_weapon_rise_pos();
  m_JumpArmatureWeaponRot = m_CurrentWeapon->get_jump_armature_weapon_rot();
}

void WeaponJumpEffect::_update_jump_data(Ref<Weapon> currentWeapon)
{
  m_JumpArmatureRiseAngFreq = currentWeapon->get_jump_armature_weapon_rise_ang_freq();
  m_JumpArmatureRiseDampingRatio = currentWeapon->get_jump_armature_weapon_rise_damping_ratio();
  m_JumpArmatureWeaponRotAngFreq = currentWeapon->get_jump_armature_weapon_rot_ang_freq();
  m_JumpArmatureWeaponRotDampingRatio = currentWeapon->get_jump_armature_weapon_rot_damping_ratio();

  m_JumpArmatureWeaponPos = currentWeapon->get_jump_armature_weapon_rise_pos();
  m_JumpArmatureWeaponRot = currentWeapon->get_jump_armature_weapon_rot();
}

void WeaponJumpEffect::_weapon_jump_effect(double delta)
{
  m_WeaponArmatureNode = m_WeaponManager->get_weapon_armature_node();
  
  if(m_MovementManager->IsJumping())
  {
    m_Jumped = true;
  }
  
  if(m_Jumped)
  {
    Vector3 weaponJumpRot = Vector3(0.0f, 0.0f, 0.0f);
    weaponJumpRot.x = Math::deg_to_rad(m_JumpArmatureWeaponRot.x);
    weaponJumpRot.y = Math::deg_to_rad(m_JumpArmatureWeaponRot.y);
    weaponJumpRot.z = Math::deg_to_rad(m_JumpArmatureWeaponRot.z);

    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_JumpArmatureRiseAngFreq, m_JumpArmatureRiseDampingRatio);
    m_DampedSpring.UpdateDampedSpringMotion(m_JumpWeaponArmatureTiltPos, m_JumpWeaponArmatureTiltPosVel, m_JumpArmatureWeaponPos);

    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_JumpArmatureWeaponRotAngFreq, m_JumpArmatureWeaponRotDampingRatio);
    m_DampedSpring.UpdateDampedSpringMotion(m_JumpWeaponArmatureTiltRot, m_JumpWeaponArmatureTiltRotVel, weaponJumpRot);
  }
  
  if(m_MovementManager->IsJumpLanded())
  {
    m_Jumped = false;

    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_JumpArmatureRiseAngFreq, m_JumpArmatureRiseDampingRatio);
    m_DampedSpring.UpdateDampedSpringMotion(m_JumpWeaponArmatureTiltPos, m_JumpWeaponArmatureTiltPosVel, Vector3(0.0f, 0.0f, 0.0f));

    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_JumpArmatureWeaponRotAngFreq, m_JumpArmatureWeaponRotDampingRatio);
    m_DampedSpring.UpdateDampedSpringMotion(m_JumpWeaponArmatureTiltRot, m_JumpWeaponArmatureTiltRotVel, Vector3(0.0f, 0.0f, 0.0f));
  }
}

void WeaponSlideEffect::_init_data(const WeaponEffectsData& weaponEffectsData)
{
  m_CharacterBody = weaponEffectsData.CharacterCompInst;
  m_MovementManager = weaponEffectsData.MovementManagerInst;
  m_CurrentWeapon = weaponEffectsData.WeaponCompInst->get_current_weapon_data();
  m_WeaponManager = weaponEffectsData.WeaponManagerInst;
  m_WeaponArmatureNode = m_WeaponManager->get_weapon_armature_node();

  if(!weaponEffectsData.CharacterCompInst || !weaponEffectsData.WeaponCompInst) {
    print_error("Character component is null!");
    return;
  }

  if(!m_CurrentWeapon.is_valid()) {
    print_error("Current weapon is not valid!");
    return;
  }

  m_SlideTiltRotVector = m_CurrentWeapon->get_slide_armature_tilt_rot();
  m_SlideDipVector = m_CurrentWeapon->get_slide_armature_dip();
  m_SlideArmatureDipTransitionValue = m_CurrentWeapon->get_slide_armature_dip_transition_value();
  m_SlideArmatureTiltAngFreq = m_CurrentWeapon->get_slide_armature_tilt_ang_freq();
  m_SlideArmatureTiltDampingRatio = m_CurrentWeapon->get_slide_armature_tilt_damping_ratio();
  m_SlideDipMaxTimer = m_CurrentWeapon->get_slide_armature_dip_reset_timer();
  m_SlideArmatureDipResetAngFreq = m_CurrentWeapon->get_slide_armature_dip_reset_ang_freq();
  m_SlideArmatureDipDampingRatio = m_CurrentWeapon->get_slide_armature_dip_reset_damping_ratio();
  m_SlideArmatureTiltEndAngFreq = m_CurrentWeapon->get_slide_armature_tilt_end_ang_freq();
  m_SlideArmatureTiltEndDampingRatio = m_CurrentWeapon->get_slide_armature_tilt_end_damping_ratio();
}

void WeaponSlideEffect::_update_slide_data(Ref<Weapon> currentWeapon)
{
  m_SlideTiltRotVector = currentWeapon->get_slide_armature_tilt_rot();
  m_SlideDipVector = currentWeapon->get_slide_armature_dip();
  m_SlideArmatureDipTransitionValue = currentWeapon->get_slide_armature_dip_transition_value();
  m_SlideArmatureTiltAngFreq = currentWeapon->get_slide_armature_tilt_ang_freq();
  m_SlideArmatureTiltDampingRatio = currentWeapon->get_slide_armature_tilt_damping_ratio();
  m_SlideDipMaxTimer = currentWeapon->get_slide_armature_dip_reset_timer();
  m_SlideArmatureDipResetAngFreq = currentWeapon->get_slide_armature_dip_reset_ang_freq();
  m_SlideArmatureDipDampingRatio = currentWeapon->get_slide_armature_dip_reset_damping_ratio();
  m_SlideArmatureTiltEndAngFreq = currentWeapon->get_slide_armature_tilt_end_ang_freq();
  m_SlideArmatureTiltEndDampingRatio = currentWeapon->get_slide_armature_tilt_end_damping_ratio();

}

void WeaponSlideEffect::_weapon_slide_effect(double delta)
{
  m_WeaponArmatureNode = m_WeaponManager->get_weapon_armature_node();

  if(m_MovementManager->IsSliding() && !m_WeaponManager->IsReloading())
  {
    m_SlideWeaponRot = m_WeaponArmatureNode->get_rotation();
    m_SlideWeaponArmaturePos = m_WeaponArmatureNode->get_position();
  
    Vector3 slideTiltVector = m_SlideTiltRotVector;
    Vector3 slideDipVector = m_SlideDipVector;
  
    slideTiltVector.x = Math::deg_to_rad(slideTiltVector.x);
    slideTiltVector.y = Math::deg_to_rad(slideTiltVector.y);
    slideTiltVector.z = Math::deg_to_rad(slideTiltVector.z);
  
    m_SlideWeaponArmaturePos = Utils::exp_decay(m_SlideWeaponArmaturePos, slideDipVector, m_SlideArmatureDipTransitionValue, delta);
  
    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_SlideArmatureTiltAngFreq, m_SlideArmatureTiltDampingRatio);
    m_DampedSpring.UpdateDampedSpringMotion(m_SlideWeaponRot, m_SlideWeaponRotVel, slideTiltVector); 
  }

  if(m_SlideDipResetTimer <= 0.0f && m_MovementManager->IsSlideStarted())
  {
    m_SlideDipResetTimer = m_SlideDipMaxTimer;
  }
  
  if(m_MovementManager->IsSlideOver())
  {
    m_SlideDipResetTimer -= delta;
  }
  
  if(m_SlideDipResetTimer <= 0.0f)
  {
    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_SlideArmatureDipResetAngFreq, m_SlideArmatureDipDampingRatio);
    m_DampedSpring.UpdateDampedSpringMotion(m_SlideWeaponArmaturePos, m_SlideWeaponArmaturePosVel, Vector3(0.0f, 0.0f, 0.0f));
  }  
  
  m_DampedSpring.CalcDampedSpringMotionParams(delta, m_SlideArmatureTiltEndAngFreq, m_SlideArmatureTiltEndDampingRatio);
  m_DampedSpring.UpdateDampedSpringMotion(m_SlideWeaponRot, m_SlideWeaponRotVel, Vector3(0.0f, 0.0f, 0.0f));
}


void WeaponEffects::_init_data(const WeaponEffectsData& weaponEffectsData)
{
  m_HoldPointNode = weaponEffectsData.HoldPointNode;
  m_WeaponManager = weaponEffectsData.WeaponManagerInst;

  m_BasePos = m_HoldPointNode->get_position();
  m_BaseRot = m_HoldPointNode->get_rotation();

  m_WeaponBobComponent._init_data(weaponEffectsData);
  m_WeaponSwayComponent._init_data(weaponEffectsData);
  m_WeaponJumpComponent._init_data(weaponEffectsData);
  m_WeaponSlideComponent._init_data(weaponEffectsData);
}

void WeaponEffects::_update_data(Ref<Weapon> currentWeapon)
{
  m_WeaponBobComponent._update_bob_data(currentWeapon);
  m_WeaponSwayComponent._update_sway_data(currentWeapon);
  m_WeaponJumpComponent._update_jump_data(currentWeapon);
  m_WeaponSlideComponent._update_slide_data(currentWeapon);
}

void WeaponEffects::_update(double delta, Vector3& sway_vel)
{
  m_WeaponSwayComponent.weapon_idle_sway(delta);
  m_WeaponSwayComponent.weapon_sway(delta, sway_vel);
  m_WeaponBobComponent.weapon_bob(delta);
  m_WeaponJumpComponent._weapon_jump_effect(delta);
  m_WeaponSlideComponent._weapon_slide_effect(delta);

  m_WeaponArmatureNode = m_WeaponManager->get_weapon_armature_node();

  if (!m_HoldPointNode) return;

  m_HoldPointNode->set_position(
    m_BasePos +
    m_WeaponSwayComponent.get_sway_offset() +
    m_WeaponBobComponent.get_weapon_bob_offset());

  m_WeaponArmatureNode->set_position(
    m_WeaponJumpComponent.get_jump_armature_tilt_offset() +
    m_WeaponSlideComponent.get_slide_armature_pos_offset()
  );

  m_WeaponArmatureNode->set_rotation(
    m_WeaponJumpComponent.get_jump_armature_tilt_rot_offset() +
    m_WeaponSlideComponent.get_slide_armature_rot_offset()
  );

  Vector3 holdPointRot = Vector3(m_BaseRot + m_WeaponSwayComponent.get_idle_sway_offset());
  m_HoldPointNode->set_rotation(holdPointRot);
}
