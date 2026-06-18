#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include "godot_cpp/classes/skeleton3d.hpp"
#include "weapon_manager.h"
#include "weapon.h"
#include "event_bus.h"

#include "components/weapon_component.h"
#include "utils/damped_spring.h"

using namespace godot;

class CameraRecoilHolder : public Node3D {
  GDCLASS(CameraRecoilHolder, Node3D);

public:
  void _ready() override;
  void _process(double delta) override;

  void addWeaponRecoil();
  void weaponReloadRotationHandler(Skeleton3D* skeleton3D, StringName reloadRootBoneName);
  void weaponReloadOver();

  ~CameraRecoilHolder();
protected:
  static void _bind_methods();

private:

  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  int m_BoneID { -1 };

  Vector3 m_TargetRot {}, m_CurrentRot {}, m_RecoilVector {};
  Vector3 m_RecoilVel {}, m_FinalRotVector {};
  Vector3 m_ReloadBoneRot {};

  Ref<RandomNumberGenerator> m_Rng { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  Skeleton3D* m_CurrentSkeleton { nullptr };

  Transform3D m_ReloadBoneTransform {};

  DampedSpring m_DampedSpring;

private:
  float m_RecoilAngFreq { 0.0f }, m_RecoilDampingRatio { 0.0f };
  bool m_IsReloading { false };
};
