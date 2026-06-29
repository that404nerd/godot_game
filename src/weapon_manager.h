#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/templates/vector.hpp>

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/decal.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/omni_light3d.hpp>

#include "components/ammo_component.h"
#include "components/character_component.h"
#include "components/muzzle_flash_component.h"
#include "components/weapon_component.h"
#include "components/weapon_effects_components.h"
#include "components/weapon_wrapper.h"

#include "globals.h"
#include "event_bus.h"
#include "godot_cpp/variant/quaternion.hpp"
#include "weapon.h"
#include "weapon_states.h"

class StateMachine;
class WeaponStateMachine;

using namespace godot;

class WeaponManager : public Node {

  GDCLASS(WeaponManager, Node);

public:
  void _ready() override;
  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _process(double delta) override;
  void _physics_process(double delta) override;
  
  WeaponStateContext& get_weapon_state_ctx() { return m_WeaponStateCtx; }
  
  ~WeaponManager();
  
public:
  void _equip_weapon();
  void _unequip_weapon();
  void _shoot_weapon(double delta);
  void _reload_weapon();
  void _weapon_switch();

  void _on_weapon_anim_finished(const StringName& anim_name);
  void _on_weapon_anim_started(const StringName& anim_name);
  
  void _weapon_unequip_over();
  void _switch_weapon_data(int weaponIndex);
  void _update_weapon_data();
  
  void generate_decal();

public:
  int get_current_weapon_ammo() { return m_AmmoComp.get_current_weapon_ammo(m_CurrentWeapon); }
  int get_current_reserve_ammo() { return m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon); }
  StringName get_current_weapon_name() { return m_CurrentWeapon->get_weaponName(); }
  double get_current_anim_length() { return m_CurrentWeaponAnimPlayer->get_current_animation_position(); }

  Ref<Weapon> get_current_weapon() { return m_CurrentWeapon; }

  Vector<Node3D*> get_weapon_nodes() { return m_WeaponNodes; }

protected:
  static void _bind_methods();

private:
  float m_TimeBetweenShots { 0.0f };

private:
  Vector3 m_MouseVel {};
  Vector2 m_ScreenCenter {};

  // TODO: Replace these two with probably an array or something else instead of vector.
  // I have to perform some unnecessary setup for stuff to work
  Vector<AnimationPlayer*> m_WeaponAnims;
  Vector<Node3D*> m_WeaponNodes;
  
  int m_WeaponIndex { 0 };
  int8_t m_CurrentWeaponState { 0 };

  AnimationPlayer* m_CurrentWeaponAnimPlayer { nullptr };
  Ref<Weapon> m_CurrentWeapon;
  Ref<PackedScene> m_DecalScene;
  PhysicsDirectSpaceState3D* m_SpaceState { nullptr };

  WeaponEffects m_WeaponEffects;
  WeaponBobComponent m_WeaponBobComponent;
  WeaponSwayComponent m_WeaponSwayComponent;
  WeaponStateContext m_WeaponStateCtx;
  AmmoComponent m_AmmoComp;
  MuzzleFlashComponent* m_MuzzleComp;
  WeaponWrapper* m_WeaponWrapperInst { nullptr };
  
  StringName m_NextWeaponName;
  StringName m_ReloadRootBoneName;

  Skeleton3D* m_Skeleton3D { nullptr };
  Transform3D m_BoneTransform {};

private:
  CharacterBody3D* m_CharacterBody { nullptr };
  Camera3D* m_Camera { nullptr };

  Ref<PhysicsRayQueryParameters3D> m_Query { nullptr };
  Dictionary m_Result;

  float m_HoldCounter { 0.0f }, m_HoldMaxTime { 0.0f };
  float m_LightTimeout { 0.05f };
  float m_TimerBetweenReloads { 0.1f };

  Vector3 m_TargetRot {}, m_CurrentRot {};

private:
  GD_DEFINE_PROPERTY(WeaponStateMachine*, weapon_state_machine, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(Node3D*, hold_point_node, nullptr);
};
