#include "camera_weapon_effects.h"

void CameraWeaponEffects::_ready()
{
  EventBus::get_singleton()->connect("weapon_fired", Callable(this, "addWeaponRecoil"));
  EventBus::get_singleton()->connect("weapon_reload_start", Callable(this, "weaponReloadRotationHandler"));
  m_Rng.instantiate();

}

void CameraWeaponEffects::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(CameraWeaponEffects, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CameraWeaponEffects, weapon_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CameraWeaponEffects, movement_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ClassDB::bind_method(D_METHOD("addWeaponRecoil"), &CameraWeaponEffects::addWeaponRecoil);
  ClassDB::bind_method(D_METHOD("weaponReloadRotationHandler", "skeleton3D"), &CameraWeaponEffects::weaponReloadRotationHandler);  
}

void CameraWeaponEffects::weaponReloadRotationHandler(Skeleton3D* skeleton3D)
{
  StringName boneName = m_CurrentWeapon->get_weaponReloadRootBoneName();
  m_BoneID = skeleton3D->find_bone(boneName);
  m_CurrentSkeleton = skeleton3D;
}

void CameraWeaponEffects::addWeaponRecoil()
{
  Vector3 recoilVec = m_CurrentWeapon->get_recoil_vector();
  m_RecoilVel = recoilVec;
}

void CameraWeaponEffects::_process(double delta)
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
  
  Vector3 eqPos {};
  
  m_CurrentWeapon = weapon_component->get_current_weapon_data();
  
  // m_DampedSpring.CalcDampedSpringMotionParams(delta, 35.0f, 0.2f);
  // m_DampedSpring.UpdateDampedSpringMotion(armatureRot, fallVel, Vector3(0.0f, 0.0f, 0.0f));
  
  if(m_BoneID != -1)
  {
    m_ReloadBoneTransform = m_CurrentSkeleton->get_bone_pose(m_BoneID);
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


  m_ReloadBoneRot = Utils::exp_decay(m_ReloadBoneRot, Vector3(0.0f, 0.0f, 0.0f), m_CurrentWeapon->get_reloadShakeResetMultiplier(), delta);
  m_ReloadBoneRot = Vector3(m_ReloadBoneRot.x, m_ReloadBoneRot.y, 0.0f);

  m_DampedSpring.CalcDampedSpringMotionParams(delta, m_CurrentWeapon->get_recoil_ang_freq(), m_CurrentWeapon->get_recoil_damping_ratio());
  m_DampedSpring.UpdateDampedSpringMotion(m_RecoilSpringRot, m_RecoilVel, eqPos);
  m_RecoilRot = m_RecoilSpringRot;

  Vector3 finalRot = Utils::exp_decay(m_RecoilRot, m_RecoilRot + (m_ReloadBoneRot * m_CurrentWeapon->get_reloadShakeSpeedMultiplier()), 15.0f, delta);

  set_rotation(finalRot);
  // m_WeaponArmatureNode->set_rotation(armatureRot);
}