#include "camera_recoil_holder.h"

void CameraRecoilHolder::_ready()
{
  EventBus::get_singleton()->connect("weapon_fired", Callable(this, "addWeaponRecoil"));
  EventBus::get_singleton()->connect("weapon_reload_start", Callable(this, "weaponReloadRotationHandler"));
  EventBus::get_singleton()->connect("weapon_reload_end", Callable(this, "weaponReloadOver"));
  m_Rng.instantiate();
}

void CameraRecoilHolder::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(CameraRecoilHolder, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ClassDB::bind_method(D_METHOD("addWeaponRecoil"), &CameraRecoilHolder::addWeaponRecoil);
  ClassDB::bind_method(D_METHOD("weaponReloadOver"), &CameraRecoilHolder::weaponReloadOver);
  ClassDB::bind_method(D_METHOD("weaponReloadRotationHandler", "skeleton3D", "reloadRootBoneName"), &CameraRecoilHolder::weaponReloadRotationHandler);  
}

void CameraRecoilHolder::weaponReloadRotationHandler(Skeleton3D* skeleton3D, StringName reloadRootBoneName)
{
  m_IsReloading = true;

  m_BoneID = skeleton3D->find_bone(reloadRootBoneName);
  m_CurrentSkeleton = skeleton3D;
}

void CameraRecoilHolder::weaponReloadOver(int currentAmmo)
{
  if((currentAmmo != m_CurrentWeapon->get_magAmmoCount() && m_CurrentWeapon->get_canSkipReload()) || currentAmmo == m_CurrentWeapon->get_magAmmoCount())
  {
    m_IsReloading = false;
  }
}

void CameraRecoilHolder::addWeaponRecoil()
{
  Vector3 recoilVec = m_CurrentWeapon->get_recoil_vector();
  m_RecoilVector = recoilVec;
  m_TargetRot += Vector3(recoilVec.x, m_Rng->randf_range(-recoilVec.y, recoilVec.y), recoilVec.z);
}

void CameraRecoilHolder::_process(double delta)
{
  if(weapon_component == nullptr)
  {
    print_error("Weapon component is null!");
    return;
  }

  Vector3 eqPos {};

  m_CurrentWeapon = weapon_component->get_current_weapon_data();

  if(m_BoneID != -1)
  {
    m_ReloadBoneTransform = m_CurrentSkeleton->get_bone_pose(m_BoneID);
  }
  
  if(m_IsReloading)
  {
    // TODO: Maybe switch to quaternions
    m_ReloadBoneRot = m_ReloadBoneTransform.basis.get_euler();
  }

  m_TargetRot = Utils::exp_decay(m_TargetRot, Vector3(0.0f, 0.0f, 0.0f), m_CurrentWeapon->get_weaponRecoilResetMultiplier(), delta);
  m_ReloadBoneRot = Utils::exp_decay(m_ReloadBoneRot, Vector3(0.0f, 0.0f, 0.0f), m_CurrentWeapon->get_reloadShakeResetMultiplier(), delta);

  m_DampedSpring.CalcDampedSpringMotionParams(delta, 0.0f, 0.0f);
  m_DampedSpring.UpdateDampedSpringMotion(m_TargetRot, m_RecoilVel, eqPos);

  m_CurrentRot = m_DampedSpring.GetUpdatedPos();
  m_RecoilVel = m_DampedSpring.GetUpdatedVel();

  m_ReloadBoneRot = Vector3(m_ReloadBoneRot.x, m_ReloadBoneRot.y, 0.0f);

  set_rotation(Utils::exp_decay(m_CurrentRot, m_CurrentRot + (m_ReloadBoneRot * m_CurrentWeapon->get_reloadShakeSpeedMultiplier()), 15.0f, delta));
}


CameraRecoilHolder::~CameraRecoilHolder()
{
}
