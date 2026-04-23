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

using namespace godot;

class WeaponManager : public Node {

  GDCLASS(WeaponManager, Node);

public:
  void _ready() override;
  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _process(double delta) override;

  ~WeaponManager();

protected:
  static void _bind_methods();

private:
  Vector3 m_MouseInput, m_MouseVel { Vector3(0.0f, 0.0f, 0.0f) };
  Vector2 m_MouseVelocity;
  uint8_t m_CurrentStateID;

  const float MOUSE_INPUT_RESET_MULTIPLIER { 10.0f };

  int m_WeaponIndex;
  String m_NextWeaponName;
private:
  GD_DEFINE_PROPERTY(WeaponBobComponent*, weapon_bob_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponSwayComponent*, weapon_sway_component, nullptr);

  GD_DEFINE_PROPERTY(Node3D*, hold_point_node, nullptr);
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(StateMachine*, character_state_machine, nullptr);
};
