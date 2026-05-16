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
  print_line("Current weapon from recoil is: ", currentWeapon->get_weaponName());
  m_TargetRot += Vector3(recoilVec.x, m_Rng->randf_range(-recoilVec.y, recoilVec.y), recoilVec.z);
}

void CameraRecoilHolder::_process(double delta)
{
  m_TargetRot = m_TargetRot.lerp(Vector3(0.0f, 0.0f, 0.0f), 6.0f * get_process_delta_time());
  m_CurrentRot = m_CurrentRot.lerp(m_TargetRot, 6.0f * get_process_delta_time());
  set_rotation(m_CurrentRot);
}