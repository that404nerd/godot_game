#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

#include <cassert>

#include "components/ammo_component.h"
#include "components/character_component.h"
#include "components/weapon_component.h"
#include "components/weapon_effects_components.h"

#include "globals.h"
#include "weapon.h"

class StateMachine;
class WeaponStateMachine;

using namespace godot;

class WeaponManager : public Node {

  GDCLASS(WeaponManager, Node);

public:
  void _ready() override;
  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _process(double delta) override;

  void _equip_weapon();
  void _unequip_weapon();
  void _shoot_weapon();
  void _reload_weapon();
  void _weapon_switch();

  void _weapon_unequip_over();
  void _switch_weapon_data();

  ~WeaponManager();

protected:
  static void _bind_methods();

private:
  Vector3 m_MouseInput;
  Vector3 m_MouseVel { Vector3(0.0f, 0.0f, 0.0f) };
  Array m_WeaponNodesGroup, m_WeaponAnimGroups;
  
  int m_WeaponIndex { 0 };

  AnimationPlayer* m_CurrentWeaponAnimPlayer { nullptr };
  Ref<Weapon> m_CurrentWeapon;

private:
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(Node3D*, hold_point_node, nullptr);

  WeaponEffects m_WeaponEffects;
  WeaponBobComponent m_WeaponBobComponent;
  WeaponSwayComponent m_WeaponSwayComponent;
  
  String m_NextWeaponName;

private:
  GD_DEFINE_PROPERTY(WeaponStateMachine*, weapon_state_machine, nullptr);
  GD_DEFINE_PROPERTY(WeaponBobComponent*, weapon_bob_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponSwayComponent*, weapon_sway_component, nullptr);
};
