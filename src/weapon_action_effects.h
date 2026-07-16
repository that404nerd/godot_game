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

  void _on_weapon_fired(Ref<Curve2D> recoilCurve);
  void _on_weapon_reload_start(Skeleton3D* skeleton3D);

  void _weapon_slide_effect(double delta);
  void _weapon_recoil_effect(double delta);
  void _weapon_reload_effect(double delta);
  void _weapon_jump_effect(double delta);

  void _update_data();

protected:
  static void _bind_methods();

private:

  GD_DEFINE_PROPERTY(MovementManager*, movement_manager, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponManager*, weapon_manager, nullptr);
  
  float m_SlideDipResetTimer { 0.0f };

  int m_BoneID { -1 };
  int m_Count { 0 };

  bool m_Jumped { false };

  Vector2 m_CurveOrigin {};

  Vector2 m_RecoilEqPos {};
  Vector3 m_RecoilSpringRot {}, m_RecoilVel {};

  Vector3 m_JumpWeaponArmatureTilt {}, m_JumpWeaponArmatureTiltVel {};
  Vector3 m_JumpWeaponArmatureTiltRot {}, m_JumpWeaponArmatureTiltRotVel {};

  Vector3 m_SlideWeaponArmaturePos {}, m_SlideWeaponArmaturePosVel {};
  Vector3 m_SlideWeaponRot {}, m_SlideWeaponRotVel {};

  Vector3 m_ReloadBoneRot {};

  Vector3 m_ArmaturePos {}, m_ArmatureRot {}, m_CurrentNodeRot {};

  Ref<Weapon> m_CurrentWeapon { nullptr };
  Ref<PackedScene> m_RecoilPatternResource { nullptr };
  Ref<Curve2D> m_Curve { nullptr };

  Skeleton3D* m_CurrentSkeleton { nullptr };
  Node3D* m_WeaponArmatureNode { nullptr };

  Transform3D m_ReloadBoneTransform {};
  DampedSpring m_DampedSpring;
};
