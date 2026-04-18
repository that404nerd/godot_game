#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/weak_ref.hpp>

#include <cassert>

#include "components/ammo_component.h"
#include "components/character_component.h"
#include "components/weapon_component.h"
#include "components/weapon_effects_components.h"

#include "globals.h"
#include "weapon.h"

class PlayerStateMachine;
class WeaponStateMachine;
class StateMachine;

using namespace godot;

class WeaponManager : public Node {

  GDCLASS(WeaponManager, Node);

private:
  struct WeaponManagerData {
    CharacterComponent* characterComponent;
    Node3D* holdPoint;
    StateMachine* stateMachine;
  };

public:
  void _init_data(const WeaponManagerData& weaponManagerData);

  void _ready() override;
  void _unhandled_input(const Ref<InputEvent>& event) override;
  void _process(double delta) override;

  WeaponComponent get_weapon_component() { return m_WeaponComponent; }
  WeaponStateMachine* get_weapon_state_machine() { return m_WeaponStateMachine; }
  
  ~WeaponManager();

protected:
  static void _bind_methods();

private:
  Vector2 m_MouseInput;
  uint8_t m_CurrentStateID;

  const float MOUSE_INPUT_RESET_MULTIPLIER { 10.0f };

  int m_WeaponIndex;
  String m_NextWeaponName;
private:
  WeaponBobComponent m_WeaponBobComponent;
  WeaponSwayComponent m_WeaponSwayComponent;
  WeaponComponent m_WeaponComponent;

  Node3D* m_HoldPointNode { nullptr };
  
  StateMachine* m_CharacterStateMachine { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };

  Array m_WeaponResourceList;
};
