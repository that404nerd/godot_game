#include "camera_recoil_holder.h"
#include "globals.h"

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
  m_RecoilVector = recoilVec;
  m_TargetRot += Vector3(recoilVec.x, m_Rng->randf_range(-recoilVec.y, recoilVec.y), recoilVec.z);
}

void CameraRecoilHolder::_process(double delta)
{
  Vector3 velocity = Vector3(0.3f, 0.8f, 1.3f);
  m_TargetRot = Utils::exp_decay(m_TargetRot, Vector3(0.0f, 0.0f, 0.0f), 7.0f, delta);

  m_DampedSpring.CalcDampedSpringMotionParams(delta, m_RecoilVector.length(), 0.7f);
  m_DampedSpring.UpdateDampedSpringMotion(m_TargetRot, velocity, Vector3(0.0f, 0.0f, 0.0f));
  m_CurrentRot = m_DampedSpring.GetUpdatedPos();
  velocity = m_DampedSpring.GetUpdatedVel();

  m_CurrentRot = Utils::exp_decay(m_CurrentRot, m_TargetRot, 20.0f, delta);

  print_line(m_CurrentRot);

  set_rotation(m_CurrentRot);
}

CameraRecoilHolder::~CameraRecoilHolder()
{
}