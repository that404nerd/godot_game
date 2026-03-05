#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/decal.hpp>
#include <godot_cpp/classes/resource_preloader.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event.hpp>


#include "globals.h"
#include "weapon.h"
#include "player.h"
#include "game_manager.h"
#include "player_state_machine.h"

using namespace godot;

class WeaponManager : public Node {

  GDCLASS(WeaponManager, Node)

public:
  WeaponManager();

  void _ready() override;
  void _init_weapon();

  void _input(const Ref<InputEvent>& event) override;

  void _physics_process(double delta) override;

  ~WeaponManager();

private:
  void _idle_weapon_sway(double delta);
  void _weapon_sway(Vector2 sway_vector);
  void _reset_weapon_sway(double delta); 
  void _weapon_bob(double delta);
  
  void _equip_weapon();

  void _shoot();
  void _generate_decal();

  void _unequip_weapon(const StringName& nextWeaponName);
  
  void _change_weapon(const StringName& weaponName);

private: // Signal stuff
  void _on_animation_finished(const StringName& anim_name);


protected:
  static void _bind_methods();

private:
  AnimationPlayer* m_WeaponAnimPlayer = nullptr;
  PhysicsDirectSpaceState3D* m_SpaceState = nullptr;

  Ref<Weapon> m_CurrentWeapon = nullptr;
  Node3D* m_WeaponNode = nullptr; // Weapon node is the actual weapon itself positioned in the weapon socket

  Array m_CurrentWeaponList; // This is the list that will have all the current weapons the player has equiped 
  Player* m_PlayerInst = nullptr;
  PlayerStateMachine* m_StateMachineInst = nullptr;

  Ref<Weapon> m_TempWeapon;
  Ref<PackedScene> m_LoadScene;

  Node3D* m_HoldPointNode = nullptr;
private:
  float m_WeaponBobTime, m_IdleWeaponBobTime;
  float m_IdleWeaponBobFreq, m_IdleWeaponBobAmp, m_WeaponBobFreq, m_WeaponBobAmp;
  float m_WeaponSwayMult, m_WeaponSwayResetValue, m_IdleWeaponBobSmoothVal, m_WeaponBobSmoothVal;
  float m_GunRange;
  
  Vector2 m_MouseInput;
  Vector2 m_ScreenCenter;

  int m_WeaponIndex;
  String m_NextWeaponName;

  StringName m_CurrentStateName;

private:
  GD_DEFINE_PROPERTY(Array, weaponList, Array());
};
