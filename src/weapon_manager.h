#pragma once

#include <godot_cpp/godot.hpp>

#include "components/ammo_component.h"
#include "components/weapon_component.h"
#include "components/weapon_effects_components.h"

#include "globals.h"
#include "weapon.h"
#include "player_state_machine.h"

using namespace godot;

// enum class WeaponStates; 

// struct WeaponAdditionalData {
//   Ref<Weapon> currentWeapon;
//   String nextWeaponName;
//   int index;
// };

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
  AnimationPlayer* m_WeaponAnimPlayer = nullptr;
  Node3D* m_HoldPointNode = nullptr;
private:
  float m_GunRange;
 
  Vector2 m_MouseInput;
  String m_CurrentStateName;

  int m_WeaponIndex;
  String m_NextWeaponName;
private:
  GD_DEFINE_PROPERTY(Array, weaponList, Array());
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponBobComponent*, weapon_bob_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponSwayComponent*, weapon_sway_component, nullptr);

private:
  Array m_CurrentWeaponList; // This is the list that will have all the current weapons the player has equiped 
  bool m_IsMovingMouse = false;
};
