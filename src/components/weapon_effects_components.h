#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/curve2d.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>

#include "../utils/damped_spring.h"
#include "../managers/movement_manager.h"
#include "character_component.h"
#include "weapon_component.h"

using namespace godot;

class WeaponManager;

struct WeaponEffectsData 
{
  Node3D *HoldPointNode, *ActionEffectsHolderNode;
  MovementManager* MovementManagerInst;
  WeaponManager* WeaponManagerInst;
  CharacterComponent* CharacterCompInst;
  WeaponComponent* WeaponCompInst;
};

class WeaponBobComponent
{
public:
  void _init_data(const WeaponEffectsData& weaponEffectsData);

  void _update_bob_data(Ref<Weapon> currentWeapon);
  void weapon_bob(double delta);

  Vector3 get_weapon_bob_offset() const { return m_BobOffset; }

private:
  Vector3 m_BobOffset {}, m_BobVector {};
  Vector3 m_CharacterVel {};

  float m_WeaponBobTime { 0.0f };
  float m_WeaponBobFreq { 0.0f };
  float m_WeaponBobAmp { 0.0f };
  float m_WeaponBobSmoothVal { 0.0f };

  Node3D* m_HoldPointNode { nullptr };
  MovementManager* m_MovementManager { nullptr };
  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
};

class WeaponSwayComponent
{
public:
  void _init_data(const WeaponEffectsData& weaponEffectsData);
  void _update_sway_data(Ref<Weapon> currentWeapon);

  void weapon_idle_sway(double delta);
  void weapon_sway(double delta, Vector3& sway_vel);

  Vector3 get_idle_sway_offset() const { return m_IdleSwayOffset; }
  Vector3 get_sway_offset() const { return m_SwayOffset; }

private:
  const float SWAY_X_MIN { -0.40f }, SWAY_X_MAX { 0.60f };
  const float SWAY_Y_MIN { -0.70f }, SWAY_Y_MAX { 0.65f };

private:
  Vector3 m_IdleSwayOffset {}, m_IdleSwayVector {};
  Vector3 m_SwayOffset {}, m_SwayVel {};
  Vector3 m_CharacterVel {};

  DampedSpring m_DampedSpring;
  
  float m_IdleWeaponBobTime { 0.0f };
  
  float m_IdleWeaponBobFreq { 0.0f };
  float m_IdleWeaponBobAmp { 0.0f };
  float m_IdleWeaponBobSmoothVal { 0.0f };
  
  float m_WeaponSpringAngFreq { 0.0f };
  float m_WeaponDampedSpringRatio { 0.0f };
  
  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  MovementManager* m_MovementManager { nullptr };
};

class WeaponSlideEffect {
public:
  void _init_data(const WeaponEffectsData& weaponEffectsData);

  void _update_slide_data(Ref<Weapon> currentWeapon);
  void _weapon_slide_effect(double delta);

  Vector3 get_slide_armature_pos_offset() { return m_SlideWeaponArmaturePos; }
  Vector3 get_slide_armature_rot_offset() { return m_SlideWeaponRot; }

private:
  Vector3 m_SlideTiltRotVector {}, m_SlideDipVector {};

  Vector3 m_SlideWeaponArmaturePos {}, m_SlideWeaponArmaturePosVel {};
  Vector3 m_SlideWeaponRot {}, m_SlideWeaponRotVel {};

  float m_SlideArmatureDipTransitionValue {};
  float m_SlideArmatureTiltAngFreq {}, m_SlideArmatureTiltDampingRatio {}, m_SlideDipMaxTimer {};
  float m_SlideArmatureDipResetAngFreq {}, m_SlideArmatureDipDampingRatio {};
  float m_SlideArmatureTiltEndAngFreq {}, m_SlideArmatureTiltEndDampingRatio {};
  float m_SlideDipResetTimer { 0.0f };

  Node3D *m_HoldPointNode { nullptr }, *m_WeaponArmatureNode { nullptr };
  MovementManager* m_MovementManager { nullptr };
  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  WeaponManager* m_WeaponManager { nullptr };

  DampedSpring m_DampedSpring {};
};

class WeaponJumpEffect {
public:
  void _init_data(const WeaponEffectsData& weaponEffectsData);

  void _update_jump_data(Ref<Weapon> currentWeapon);
  void _weapon_jump_effect(double delta);

  Vector3 get_jump_armature_tilt_pos_offset() { return m_JumpWeaponArmatureTiltPos; }
  Vector3 get_jump_armature_tilt_rot_offset() { return m_JumpWeaponArmatureTiltRot; }
private:

  bool m_Jumped { false };

  Vector3 m_JumpArmatureWeaponPos {}, m_JumpArmatureWeaponRot {};

  Vector3 m_JumpWeaponArmatureTiltPos {}, m_JumpWeaponArmatureTiltPosVel {};
  Vector3 m_JumpWeaponArmatureTiltRot {}, m_JumpWeaponArmatureTiltRotVel {};

  float m_JumpArmatureRiseAngFreq {}, m_JumpArmatureRiseDampingRatio {};
  float m_JumpArmatureWeaponRotAngFreq {}, m_JumpArmatureWeaponRotDampingRatio {};

  Node3D *m_HoldPointNode { nullptr }, *m_WeaponArmatureNode { nullptr };
  WeaponManager* m_WeaponManager { nullptr };
  MovementManager* m_MovementManager { nullptr };
  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };

  DampedSpring m_DampedSpring {};
};

class WeaponActionEffects : public Node {
  GDCLASS(WeaponActionEffects, Node);

public:
  void _init_data(const WeaponEffectsData& weaponEffectsData);
  void _update(double delta);

  void _on_weapon_fired(Ref<Curve2D> recoilCurve);
  void _on_weapon_reload_start(Skeleton3D* skeleton3D);
  void _on_weapon_switched(Ref<Weapon> currentWeapon);

  void _weapon_recoil_effect(double delta);
  void _weapon_reload_effect(double delta);

  Vector3 get_actions_effects_rot() { return m_WeaponActionEffectsRot; };

protected:
  static void _bind_methods();

private:
  int m_BoneID { -1 };
  int m_Count { 0 };

  
  Vector2 m_CurveOrigin {};
  
  Vector2 m_RecoilEqPos {};
  Vector3 m_RecoilSpringRot {}, m_RecoilVel {};
  
  Vector3 m_ReloadBoneRot {};
  
  Vector3 m_WeaponActionEffectsRot {};
  Transform3D m_ReloadBoneTransform {};
  DampedSpring m_DampedSpring;
  
  Ref<Curve2D> m_RecoilCurve { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  
  Skeleton3D* m_CurrentSkeleton { nullptr };
  WeaponManager* m_WeaponManager { nullptr };
  WeaponComponent* m_WeaponComponent { nullptr };

};


class WeaponEffects : public Node
{
  GDCLASS(WeaponEffects, Node);
public:
  void _init();
  void _init_data(const WeaponEffectsData& weaponEffectsData);

  void _on_weapon_switched(Ref<Weapon> currentWeapon);

  void _update_data(Ref<Weapon> currentWeapon);
  void _update(double delta);

  void _exit_tree() override;

protected:
  static void _bind_methods();

private:
  Node3D *m_WeaponArmatureNode { nullptr };
  Vector3 m_BasePos {}, m_BaseRot {};
  
  Vector3 m_ArmaturePos {}, m_ArmatureRot {}, m_MouseVel {}, m_WeaponActionsEffectsRot {};
  
  WeaponActionEffects* m_WeaponActionsEffectsComp { nullptr };
  WeaponBobComponent m_WeaponBobComponent;
  WeaponSwayComponent m_WeaponSwayComponent;
  WeaponSlideEffect m_WeaponSlideComponent;
  WeaponJumpEffect m_WeaponJumpComponent;

private:
  GD_DEFINE_PROPERTY(Node3D*, hold_point_node, nullptr);
  GD_DEFINE_PROPERTY(Node3D*, weapon_actions_effect_holder, nullptr);
  GD_DEFINE_PROPERTY(MovementManager*, movement_manager, nullptr);
  GD_DEFINE_PROPERTY(InputCommandSystem*, input_command_system, nullptr);
  GD_DEFINE_PROPERTY(WeaponManager*, weapon_manager, nullptr);
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
};