#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>
#include <godot_cpp/classes/path2d.hpp>
#include <godot_cpp/classes/curve2d.hpp>

#include "weapon_manager.h"
#include "weapon_state_machine.h"
#include "resources/weapon.h"
#include "singletons/event_bus.h"

#include "components/weapon_component.h"
#include "utils/damped_spring.h"

using namespace godot;

class WeaponActionEffects : public Node3D {
  GDCLASS(WeaponActionEffects, Node3D);

public:
  void _ready() override;
  void _process(double delta) override;

  void addWeaponRecoil(Ref<Curve2D> recoilCurve);
  void weaponReloadRotationHandler(Skeleton3D* skeleton3D);

  void _weapon_slide_effect(double delta);

protected:
  static void _bind_methods();

private:

  GD_DEFINE_PROPERTY(MovementManager*, movement_manager, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponManager*, weapon_manager, nullptr);
  
  int m_BoneID { -1 };
  int m_Count { 0 };

  Vector2 m_CurveOrigin {};

  Vector2 m_RecoilEqPos {};
  Vector3 m_RecoilSpringRot {}, m_SlideWeaponRot {};
  Vector3 m_RecoilVel {};
  Vector3 m_ReloadBoneRot {};

  Ref<RandomNumberGenerator> m_Rng { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  Ref<PackedScene> m_RecoilPatternResource { nullptr };
  Ref<Curve2D> m_Curve { nullptr };

  Skeleton3D* m_CurrentSkeleton { nullptr };
  Node3D* m_WeaponArmatureNode { nullptr };

  Transform3D m_ReloadBoneTransform {};

  DampedSpring m_DampedSpring;
};
