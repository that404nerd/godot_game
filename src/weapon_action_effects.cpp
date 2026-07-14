#include "weapon_action_effects.h"

void WeaponActionEffects::_ready()
{
  EventBus::get_singleton()->connect("weapon_fired", Callable(this, "addWeaponRecoil"));
  EventBus::get_singleton()->connect("weapon_reload_start", Callable(this, "weaponReloadRotationHandler"));
  m_Rng.instantiate();
}

void WeaponActionEffects::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponActionEffects, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponActionEffects, weapon_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponActionEffects, movement_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ClassDB::bind_method(D_METHOD("addWeaponRecoil", "recoilPatternResource"), &WeaponActionEffects::addWeaponRecoil);
  ClassDB::bind_method(D_METHOD("weaponReloadRotationHandler", "skeleton3D"), &WeaponActionEffects::weaponReloadRotationHandler);  
}

void WeaponActionEffects::weaponReloadRotationHandler(Skeleton3D* skeleton3D)
{
  StringName boneName = m_CurrentWeapon->get_weaponReloadRootBoneName();
  m_BoneID = skeleton3D->find_bone(boneName);
  m_CurrentSkeleton = skeleton3D;
}

void WeaponActionEffects::addWeaponRecoil(Ref<Curve2D> recoilCurve)
{
  m_Curve = recoilCurve;

  m_RecoilEqPos = m_Curve->get_point_position(m_Count) - m_CurveOrigin;
  m_Count++;
}

void WeaponActionEffects::_weapon_slide_effect(double delta)
{
  // m_SlideWeaponRot = m_WeaponArmatureNode->get_rotation();
  // Vector3 slide_tilt_rotation = Vector3(2.0f, 2.0f, -5.0f);

  // Vector3 bobVector = Vector3(0.0f, 0.0f, 0.0f);
  // bobVector.z = Math::deg_to_rad(slide_tilt_rotation.z);

  // m_SlideWeaponRot = Vector3(
  //   Utils::exp_decay(m_SlideWeaponRot.x, bobVector.x, 
  //                     5.0f, (float)delta),
  //   Utils::exp_decay(m_SlideWeaponRot.y, bobVector.y,
  //                   5.0f, (float)delta), 
  //   Utils::exp_decay(m_SlideWeaponRot.z, Math::deg_to_rad(slide_tilt_rotation.z), 
  //                     5.0f, (float)delta)
  // );
}

void WeaponActionEffects::_process(double delta)
{
  if(weapon_component == nullptr || weapon_manager == nullptr)
  {
    print_error("Weapon component or weapon manager is null!");
    return;
  }

  m_WeaponArmatureNode = weapon_manager->get_weapon_armature_node();
  // Vector3 armatureRot = m_WeaponArmatureNode->get_rotation();
  // Vector3 jumpVel = Vector3(-1.2f, 0.0f, -1.2f);
  // Vector3 fallVel = Vector3(0.0f, 0.0f, 0.0f);
  
  m_CurrentWeapon = weapon_component->get_current_weapon_data();

  if(weapon_manager)
  {
    // Shift everything to (0, 0)
    m_Curve = weapon_manager->get_recoil_curve();
    m_CurveOrigin = m_Curve->get_point_position(0);

    m_RecoilEqPos = m_Curve->get_point_position(m_Count) - m_CurveOrigin;
  }
  
  // // m_DampedSpring.CalcDampedSpringMotionParams(delta, 35.0f, 0.2f);
  // // m_DampedSpring.UpdateDampedSpringMotion(armatureRot, fallVel, Vector3(0.0f, 0.0f, 0.0f));
  
  if(m_BoneID != -1)
  {
    m_ReloadBoneTransform = m_CurrentSkeleton->get_bone_pose(m_BoneID);
  }

  // // if(movement_manager->IsSliding())
  // // {
  // //   _weapon_slide_effect(delta);
  // // }


  if(m_Curve.is_valid())
  {
    m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_recoil_ang_freq(), m_CurrentWeapon->get_recoil_damping_ratio());
    m_DampedSpring.UpdateDampedSpringMotion(m_RecoilSpringRot, m_RecoilVel, Vector3(-(m_RecoilEqPos.y * m_CurrentWeapon->get_recoilMultiplier()), 
                                                                                    -(m_RecoilEqPos.x * m_CurrentWeapon->get_recoilMultiplier()), 0.0f));

    if(m_Count >= m_Curve->get_point_count() - 1 || !weapon_manager->IsWeaponFiring())
    {
      m_Count = 0;
      m_RecoilEqPos = m_Curve->get_point_position(m_Count) - m_CurveOrigin;
    }
  }

  // if(movement_state_machine->get_current_state() == static_cast<int>(MovementStates::FALL))
  // {
  //   m_DampedSpring.CalcDampedSpringMotionParams(delta, 40.0f, 0.4f);
  //   m_DampedSpring.UpdateDampedSpringMotion(armatureRot, jumpVel, Vector3(Math::deg_to_rad(2.0f), 0.0f, Math::deg_to_rad(-1.0f)));
  // }
  
  if(weapon_manager->get_weapon_state_ctx().IsReloading)
  {
    m_ReloadBoneRot = m_ReloadBoneTransform.basis.get_euler();
  }

  m_SlideWeaponRot = Utils::exp_decay(m_SlideWeaponRot, Vector3(0.0f, 0.0f, 0.0f), 5.0f, delta);

  m_ReloadBoneRot = Utils::exp_decay(m_ReloadBoneRot, Vector3(0.0f, 0.0f, 0.0f), m_CurrentWeapon->get_reloadShakeResetMultiplier(), delta);
  m_ReloadBoneRot = Vector3(m_ReloadBoneRot.x, m_ReloadBoneRot.y, 0.0f);

  Vector3 finalRot = Utils::exp_decay(m_RecoilSpringRot, m_RecoilSpringRot + (m_ReloadBoneRot * m_CurrentWeapon->get_reloadShakeSpeedMultiplier()), 15.0f, delta);
  set_rotation(finalRot);
  // m_WeaponArmatureNode->set_rotation(m_SlideWeaponRot);
}