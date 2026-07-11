#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>

#include "weapon_manager.h"
#include "weapon_state_machine.h"
#include "resources/weapon.h"
#include "singletons/event_bus.h"

#include "components/weapon_component.h"
#include "utils/damped_spring.h"

using namespace godot;

class WeaponEffectsHolder : public Node3D {
  GDCLASS(WeaponEffectsHolder, Node3D);

public:
  void _ready() override;
  void _process(double delta) override;

  void addWeaponRecoil();
  void weaponReloadRotationHandler(Skeleton3D* skeleton3D);

  void _weapon_slide_effect(double delta);

protected:
  static void _bind_methods();

private:

  GD_DEFINE_PROPERTY(MovementManager*, movement_manager, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponManager*, weapon_manager, nullptr);
  
  int m_BoneID { -1 };

  Vector3 m_RecoilRot {}, m_RecoilSpringRot {}, m_SlideWeaponRot {};
  Vector3 m_RecoilVel {};
  Vector3 m_ReloadBoneRot {};

  Ref<RandomNumberGenerator> m_Rng { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  Skeleton3D* m_CurrentSkeleton { nullptr };
  Node3D* m_WeaponArmatureNode { nullptr };

  Transform3D m_ReloadBoneTransform {};

  DampedSpring m_DampedSpring;
};
