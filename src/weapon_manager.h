#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/decal.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include "components/ammo_component.h"
#include "components/character_component.h"
#include "components/weapon_component.h"
#include "components/weapon_effects_components.h"

#include "globals.h"
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
  
  const WeaponStateContext& get_weapon_state_ctx() { return m_WeaponStateCtx; }
  
  ~WeaponManager();
  
public:
  void _equip_weapon();
  void _unequip_weapon();
  void _shoot_weapon(double delta);
  void _reload_weapon();
  void _weapon_switch();
  
  void _weapon_unequip_over();
  void _switch_weapon_data(int weaponIndex);
  
  void generate_decal();

protected:
  static void _bind_methods();

private:
  Vector3 m_MouseInput {};
  Vector3 m_MouseVel {};
  Vector2 m_ScreenCenter {};

  Array m_WeaponNodesGroup, m_WeaponAnimGroups;
  
  int m_WeaponIndex { 0 };

  AnimationPlayer* m_CurrentWeaponAnimPlayer { nullptr };
  Ref<Weapon> m_CurrentWeapon;
  Ref<PackedScene> m_LoadScene;
  PhysicsDirectSpaceState3D* m_SpaceState { nullptr };

  WeaponEffects m_WeaponEffects;
  WeaponBobComponent m_WeaponBobComponent;
  WeaponSwayComponent m_WeaponSwayComponent;
  WeaponStateContext m_WeaponStateCtx;
  AmmoComponent m_AmmoComp;
  
  String m_NextWeaponName;

private:
  CharacterBody3D* m_CharacterBody { nullptr };
  Camera3D* m_Camera { nullptr };

  Ref<PhysicsRayQueryParameters3D> m_Query { nullptr };
  Dictionary m_Result;

  float m_TimeBeforeShoot { 0.0f };

private:
  GD_DEFINE_PROPERTY(WeaponStateMachine*, weapon_state_machine, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(Node3D*, hold_point_node, nullptr);

};
