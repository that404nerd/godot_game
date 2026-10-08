#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/decal.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/base_material3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include "../input_command_system.h"
#include "../components/ammo_component.h"
#include "../components/character_component.h"
#include "../components/muzzle_flash_component.h"
#include "../components/weapon_component.h"
#include "../components/weapon_effects_components.h"
#include "../components/weapon_wrapper.h"
#include "../components/health_component.h"
#include "../components/play_anim_component.h"

#include "../dd3d_cpp_api.hpp"

#include "../globals.h"
#include "../singletons/event_bus.h"
#include "../state_machines/movement_state_machine.h"
#include "../resources/weapon.h"
#include "../states/weapon_states.h"
#include "../state_machines/weapon_state_machine.h"

using namespace godot;

class WeaponManager : public Node {

  GDCLASS(WeaponManager, Node);

public:
  void _init();
  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _update(double delta);
  void _physics_update(double delta);
 
public:

  void _init_weapons();
  void _init_weapon_anim_connections();
  void _init_weapon_manager_data();
  void _change_fov();

  void _equip_weapon();
  void _unequip_weapon();
  
  void _shoot_weapon(double delta);
  void _shoot_weapon_over();

  void _reload_weapon();
  void _weapon_switch();

  void _on_weapon_anim_finished(const StringName& anim_name);
  void _on_weapon_anim_started(const StringName& anim_name);

  void _weapon_unequip_over();
  void _switch_weapon_data(int weaponIndex);
  void _update_weapon_data(Ref<Weapon> nextWeapon);
  
  void generate_decal();

public:

  bool IsShooting() { return m_WeaponStateCtx.IsShooting; }
  bool IsReloading() { return m_WeaponStateCtx.IsReloading; }

  bool current_weapon_has_auto_reload() {
    Ref<Weapon> currentWeapon = get_current_weapon();
    return currentWeapon->get_auto_reload();
  }

  WeaponStateContext& get_weapon_state_ctx() { return m_WeaponStateCtx; }
  float get_time_between_shots() { return m_TimeBetweenShots; }

  int get_current_weapon_ammo() { return m_AmmoComp.get_current_weapon_ammo(m_CurrentWeapon); }
  int get_current_reserve_ammo() { return m_AmmoComp.get_current_weapon_reserve_ammo(m_CurrentWeapon); }
  StringName get_current_weapon_name() { return m_CurrentWeapon->get_weaponName(); }

  Ref<Weapon> get_current_weapon() { return m_CurrentWeapon; }

  Node3D* get_weapon_armature_node() { return m_WeaponWrapperInst->get_armature_node(); }
  Skeleton3D* get_armature_skeleton() { return m_Skeleton3D; }

  WeaponWrapper* get_weapon_wrapper_inst() { return m_WeaponWrapperInst; }

  Vector<Node3D*> get_weapon_nodes() { return m_WeaponNodes; }

protected:
  static void _bind_methods();

private:
  // Ref<PackedScene> m_RecoilResource { nullptr };
  // Ref<Curve2D> m_RecoilCurve { nullptr };
  // Node* m_RecoilPathNode { nullptr };
  // Path2D* m_RecoilPath { nullptr };
  
  Ref<StandardMaterial3D> m_StdMaterial { nullptr };

  Vector<PlayAnimComponent*> m_WeaponCompList;
  Vector<Node3D*> m_WeaponNodes;
  Vector<Node3D*> m_WeaponSceneNodes;

  PhysicsDirectSpaceState3D* m_SpaceState { nullptr };
  Ref<PhysicsRayQueryParameters3D> m_Query { nullptr };

  Ref<Weapon> m_CurrentWeapon { nullptr };
  Ref<PackedScene> m_DecalScene { nullptr };

  Marker3D* m_WeaponMuzzleMarker { nullptr };
  Node* m_BulletDecalInstNode { nullptr };
  Decal* m_BulletDecalNode { nullptr };

  WeaponStateContext m_WeaponStateCtx;
  AmmoComponent m_AmmoComp;

  MuzzleFlashComponent* m_MuzzleComp { nullptr };
  WeaponWrapper* m_WeaponWrapperInst { nullptr };
  
  Skeleton3D* m_Skeleton3D { nullptr };
  CharacterBody3D* m_CharacterBody { nullptr };
  Dictionary m_Result;

private:
  float m_TimeBetweenShots { 0.0f };
  int m_WeaponIndex { 0 };

  float m_MuzzleLightTimeout { 0.0f };
  float m_HoldCounter { 0.0f }, m_HoldMaxTime { 0.0f };

  Vector3 m_TargetRot {}, m_CurrentRot {};

  const float MAX_SHOOT_STATE_TIME { 1.0f };

private:
  GD_DEFINE_PROPERTY(InputCommandSystem*, input_command_system, nullptr);
  GD_DEFINE_PROPERTY(WeaponStateMachine*, weapon_state_machine, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(PlayAnimComponent*, play_anim_component, nullptr);
  GD_DEFINE_PROPERTY(HealthComponent*, health_component, nullptr);
  GD_DEFINE_PROPERTY(Node3D*, hold_point_node, nullptr);

  GD_DEFINE_PROPERTY(bool, weapons_init_required, false);
  GD_DEFINE_PROPERTY(bool, weapon_fov_override_required, false);
  GD_DEFINE_PROPERTY(bool, animations_from_weapon, false);
};