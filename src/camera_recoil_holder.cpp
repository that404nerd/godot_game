#include "camera_recoil_holder.h"

void CameraRecoilHolder::_ready()
{
  EventBus::get_singleton()->connect("weapon_fired", Callable(this, "addWeaponRecoil"));
  EventBus::get_singleton()->connect("weapon_reload_start", Callable(this, "reloadWeaponCameraEffect"));
  m_Rng.instantiate();
}

void CameraRecoilHolder::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(CameraRecoilHolder, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ClassDB::bind_method(D_METHOD("addWeaponRecoil"), &CameraRecoilHolder::addWeaponRecoil);  
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
  m_RecoilAngFreq = m_CurrentWeapon->get_recoil_ang_freq();
  m_RecoilDampingRatio = m_CurrentWeapon->get_recoil_damping_ratio();

  m_TargetRot = Utils::exp_decay(m_TargetRot, Vector3(0.0f, 0.0f, 0.0f), 7.0f, delta);

  m_DampedSpring.CalcDampedSpringMotionParams(delta, 0.0f, 0.0f);
  m_DampedSpring.UpdateDampedSpringMotion(m_TargetRot, m_RecoilVel, eqPos);

  m_CurrentRot = m_DampedSpring.GetUpdatedPos();
  m_RecoilVel = m_DampedSpring.GetUpdatedVel();

  set_rotation(m_CurrentRot);
}

CameraRecoilHolder::~CameraRecoilHolder()
{
}
