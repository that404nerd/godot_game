#pragma once

#include "camera_recoil_holder.h"

#include "weapon.h"

#include "event_bus.h"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class CameraRecoilHolder : public Node3D {
  GDCLASS(CameraRecoilHolder, Node3D);

public:
  void _ready() override;
  void _process(double delta) override;

  void addWeaponRecoil(Ref<Weapon> currentWeapon);

protected:
  static void _bind_methods();

private:
  Vector3 m_TargetRot {}, m_CurrentRot {};
  Ref<RandomNumberGenerator> m_Rng;
};