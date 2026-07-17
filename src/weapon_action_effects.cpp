#include "weapon_action_effects.h"

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

void WeaponActionEffects::_update_data()
{
  m_CurrentNodeRot = m_RecoilSpringRot + (m_ReloadBoneRot * m_CurrentWeapon->get_reloadShakeSpeedMultiplier());

  set_rotation(m_CurrentNodeRot);
}

void WeaponActionEffects::_process(double delta)
{
  if(weapon_component == nullptr || weapon_manager == nullptr)
  {
    print_error("Weapon component or weapon manager is null!");
    return;
  }

  m_CurrentWeapon = weapon_component->get_current_weapon_data();

  _weapon_recoil_effect(delta);
  _weapon_reload_effect(delta);

  _update_data();
}