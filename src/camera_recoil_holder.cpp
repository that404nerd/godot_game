#include "camera_recoil_holder.h"

void CameraRecoilHolder::_ready()
{
  EventBus::get_singleton()->connect("weapon_fired", Callable(this, "addWeaponRecoil"));
  m_Rng.instantiate();
}

void CameraRecoilHolder::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("addWeaponRecoil", "currentWeapon"), &CameraRecoilHolder::addWeaponRecoil);  
}

void CameraRecoilHolder::addWeaponRecoil(Ref<Weapon> currentWeapon)
{
  Vector3 recoilVec = currentWeapon->get_recoil_vector();
  m_TargetRot += Vector3(recoilVec.x, m_Rng->randf_range(-recoilVec.y, recoilVec.y), recoilVec.z);
}

void CameraRecoilHolder::_process(double delta)
{
  m_TargetRot = Utils::exp_decay(m_TargetRot, Vector3(0.0f, 0.0f, 0.0f), 7.0f, delta);
  m_CurrentRot = Utils::exp_decay(m_CurrentRot, m_TargetRot, 8.0f, delta);
  set_rotation(m_CurrentRot);
}