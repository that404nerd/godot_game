#include "weapon_action_effects.h"
#include "godot_cpp/core/math.hpp"

void WeaponActionEffects::_ready()
{
  EventBus::get_singleton()->connect("weapon_fired", Callable(this, "_on_weapon_fired"));
  EventBus::get_singleton()->connect("weapon_reload_start", Callable(this, "_on_weapon_reload_start"));
}

void WeaponActionEffects::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponActionEffects, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponActionEffects, weapon_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponActionEffects, movement_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ClassDB::bind_method(D_METHOD("_on_weapon_fired", "recoilPatternResource"), &WeaponActionEffects::_on_weapon_fired);
  ClassDB::bind_method(D_METHOD("_on_weapon_reload_start", "skeleton3D"), &WeaponActionEffects::_on_weapon_reload_start);  
}

void WeaponActionEffects::_on_weapon_reload_start(Skeleton3D* skeleton3D)
{
  StringName boneName = m_CurrentWeapon->get_weaponReloadRootBoneName();
  m_BoneID = skeleton3D->find_bone(boneName);
  m_CurrentSkeleton = skeleton3D;
}

void WeaponActionEffects::_on_weapon_fired(Ref<Curve2D> recoilCurve)
{
  m_Curve = recoilCurve;

  m_RecoilEqPos = m_Curve->get_point_position(m_Count) - m_CurveOrigin;
  m_Count++;
}

void WeaponActionEffects::_weapon_recoil_effect(double delta)
{
  if(weapon_manager)
  {
    // Shift everything to (0, 0)
    m_Curve = weapon_manager->get_recoil_curve();
    m_CurveOrigin = m_Curve->get_point_position(0);

    m_RecoilEqPos = m_Curve->get_point_position(m_Count) - m_CurveOrigin;
  }

  if(m_Curve.is_valid())
  {
    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_recoil_ang_freq(), m_CurrentWeapon->get_recoil_damping_ratio());
    m_DampedSpring.UpdateDampedSpringMotion(m_RecoilSpringRot, m_RecoilVel, Vector3(-(m_RecoilEqPos.y * m_CurrentWeapon->get_recoilMultiplier()), 
                                                                                    -(m_RecoilEqPos.x * m_CurrentWeapon->get_recoilMultiplier()), 0.0f));

    if(m_Count >= m_Curve->get_point_count() - 1 || !weapon_manager->IsWeaponFiring())
    {
      m_Count = 0;
      m_RecoilEqPos = m_Curve->get_point_position(m_Count);
    }
  }
}

void WeaponActionEffects::_weapon_reload_effect(double delta)
{
  if(m_BoneID != -1)
  {
    m_ReloadBoneTransform = m_CurrentSkeleton->get_bone_pose(m_BoneID);
  }

  if(weapon_manager->get_weapon_state_ctx().IsReloading)
  {
    m_ReloadBoneRot = m_ReloadBoneTransform.basis.get_euler();
  }
 
  m_ReloadBoneRot = Utils::exp_decay(m_ReloadBoneRot, Vector3(0.0f, 0.0f, 0.0f), m_CurrentWeapon->get_reloadShakeResetMultiplier(), delta);
  m_ReloadBoneRot = Vector3(m_ReloadBoneRot.x, m_ReloadBoneRot.y, 0.0f);
}

void WeaponActionEffects::_weapon_jump_effect(double delta)
{
  if(movement_manager->IsJumping())
  {
    m_Jumped = true;
  }
  
  if(m_Jumped)
  {
    Vector3 weaponJumpRot = Vector3(0.0f, 0.0f, 0.0f);
    weaponJumpRot.x = Math::deg_to_rad(m_CurrentWeapon->get_jump_armature_weapon_rot().x);
    weaponJumpRot.y = Math::deg_to_rad(m_CurrentWeapon->get_jump_armature_weapon_rot().y);
    weaponJumpRot.z = Math::deg_to_rad(m_CurrentWeapon->get_jump_armature_weapon_rot().z);

    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_jump_armature_weapon_rise_ang_freq(), m_CurrentWeapon->get_jump_armature_weapon_rise_damping_ratio());
    m_DampedSpring.UpdateDampedSpringMotion(m_JumpWeaponArmatureTilt, m_JumpWeaponArmatureTiltVel, m_CurrentWeapon->get_jump_armature_weapon_rise_pos());

    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_jump_armature_weapon_rot_ang_freq(), m_CurrentWeapon->get_jump_armature_weapon_rot_damping_ratio());
    m_DampedSpring.UpdateDampedSpringMotion(m_JumpWeaponArmatureTiltRot, m_JumpWeaponArmatureTiltRotVel, weaponJumpRot);
  }
  
  if(movement_manager->IsJumpLanded())
  {
    m_Jumped = false;

    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_jump_armature_weapon_rise_ang_freq(), m_CurrentWeapon->get_jump_armature_weapon_rise_damping_ratio());
    m_DampedSpring.UpdateDampedSpringMotion(m_JumpWeaponArmatureTilt, m_JumpWeaponArmatureTiltVel, Vector3(0.0f, 0.0f, 0.0f));

    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_jump_armature_weapon_rot_ang_freq(), m_CurrentWeapon->get_jump_armature_weapon_rot_damping_ratio());
    m_DampedSpring.UpdateDampedSpringMotion(m_JumpWeaponArmatureTiltRot, m_JumpWeaponArmatureTiltRotVel, Vector3(0.0f, 0.0f, 0.0f));
  }
}

void WeaponActionEffects::_weapon_slide_effect(double delta)
{
  if(movement_manager->IsSliding() && !weapon_manager->IsReloading())
  {
    m_SlideWeaponRot = m_WeaponArmatureNode->get_rotation();
    m_SlideWeaponArmaturePos = m_WeaponArmatureNode->get_position();
  
    Vector3 slideTiltVector = m_CurrentWeapon->get_slide_armature_tilt_rot();
    Vector3 slideDipVector = m_CurrentWeapon->get_slide_armature_dip();
  
    slideTiltVector.x = Math::deg_to_rad(slideTiltVector.x);
    slideTiltVector.y = Math::deg_to_rad(slideTiltVector.y);
    slideTiltVector.z = Math::deg_to_rad(slideTiltVector.z);
  
    m_SlideWeaponArmaturePos = Utils::exp_decay(m_SlideWeaponArmaturePos, slideDipVector, m_CurrentWeapon->get_slide_armature_dip_transition_value(), delta);
  
    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_slide_armature_tilt_ang_freq(), m_CurrentWeapon->get_slide_armature_tilt_damping_ratio());
    m_DampedSpring.UpdateDampedSpringMotion(m_SlideWeaponRot, m_SlideWeaponRotVel, slideTiltVector); 
  }

  if(m_SlideDipResetTimer <= 0.0f && movement_manager->IsSlideStarted())
  {
    m_SlideDipResetTimer = m_CurrentWeapon->get_slide_armature_dip_reset_timer();
  }
  
  if(movement_manager->IsSlideOver())
  {
    m_SlideDipResetTimer -= delta;
  }
  
  if(m_SlideDipResetTimer <= 0.0f)
  {
    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_slide_armature_dip_reset_ang_freq(), m_CurrentWeapon->get_slide_armature_dip_reset_damping_ratio());
    m_DampedSpring.UpdateDampedSpringMotion(m_SlideWeaponArmaturePos, m_SlideWeaponArmaturePosVel, Vector3(0.0f, 0.0f, 0.0f));
  }  
  
  m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_slide_armature_tilt_end_ang_freq(), m_CurrentWeapon->get_slide_armature_tilt_end_damping_ratio());
  m_DampedSpring.UpdateDampedSpringMotion(m_SlideWeaponRot, m_SlideWeaponRotVel, Vector3(0.0f, 0.0f, 0.0f));
}

void WeaponActionEffects::_update_data()
{
  m_CurrentNodeRot = m_RecoilSpringRot + (m_ReloadBoneRot * m_CurrentWeapon->get_reloadShakeSpeedMultiplier());

  m_ArmaturePos = m_JumpWeaponArmatureTilt + m_SlideWeaponArmaturePos;
  m_ArmatureRot = m_JumpWeaponArmatureTiltRot + m_SlideWeaponRot;

  set_rotation(m_CurrentNodeRot);

  m_WeaponArmatureNode->set_position(m_ArmaturePos);
  m_WeaponArmatureNode->set_rotation(m_ArmatureRot);
}

void WeaponActionEffects::_process(double delta)
{
  if(weapon_component == nullptr || weapon_manager == nullptr)
  {
    print_error("Weapon component or weapon manager is null!");
    return;
  }

  m_WeaponArmatureNode = weapon_manager->get_weapon_armature_node();
  m_CurrentWeapon = weapon_component->get_current_weapon_data();

  _weapon_slide_effect(delta);
  _weapon_recoil_effect(delta);
  _weapon_reload_effect(delta);
  _weapon_jump_effect(delta);

  _update_data();
}