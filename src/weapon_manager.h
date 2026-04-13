#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

#include "components/ammo_component.h"
#include "components/character_component.h"
#include "components/weapon_component.h"
#include "components/weapon_effects_components.h"

#include "globals.h"
#include "weapon.h"
#include "state_machine.h"

using namespace godot;

class WeaponManager : public Node {

public:
  void _init_data(CharacterComponent* characterComponent, Node3D* holdPoint, StateMachine* stateMachine);

  void _ready() override;
  
  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _process(double delta) override;

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
  WeaponBobComponent m_WeaponBobComponent;
  WeaponSwayComponent m_WeaponSwayComponent;
  WeaponComponent m_WeaponComponent;

  Node3D* m_HoldPointNode { nullptr };
  StateMachine* m_PlayerStateMachine { nullptr };

  Array m_WeaponResourceList;
};
