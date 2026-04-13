#pragma once

#include <godot_cpp/godot.hpp>

#include "components/ammo_component.h"
#include "components/weapon_component.h"
#include "components/weapon_effects_components.h"

#include "globals.h"
#include "weapon.h"
#include "state_machine.h"

using namespace godot;

class WeaponManager : public Node {

  GDCLASS(WeaponManager, Node)

private:

public:
  WeaponManager();

  void _ready() override;
  
  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _process(double delta) override;

  ~WeaponManager();

protected:
  static void _bind_methods();

private:
  // AnimationPlayer* m_WeaponAnimPlayer = nullptr;
private:
  Vector2 m_MouseInput;
  String m_CurrentStateName;

  const float MOUSE_INPUT_RESET_MULTIPLIER = 10.0f;

  int m_WeaponIndex;
  String m_NextWeaponName;
private:
  GD_DEFINE_PROPERTY(Array, weaponList, Array());
  GD_DEFINE_PROPERTY(StateMachine*, player_state_machine, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponBobComponent*, weapon_bob_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponSwayComponent*, weapon_sway_component, nullptr);
};
