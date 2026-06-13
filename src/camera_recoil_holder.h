#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/node3d.hpp>

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

  ~CameraRecoilHolder();
protected:
  static void _bind_methods();

private:

  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);

  Vector3 m_TargetRot {}, m_CurrentRot {}, m_RecoilVector {};
  Ref<RandomNumberGenerator> m_Rng { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  Vector3 m_RecoilVel {};

  DampedSpring m_DampedSpring;

private:
  float m_RecoilAngFreq {}, m_RecoilDampingRatio {};
};
