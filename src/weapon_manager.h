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

  void _unhandled_input(const Ref<InputEvent>& event) override;

  void _physics_process(double delta) override;
  void _weapon_bob(double delta);

  void _shoot();

  ~WeaponManager();

protected:
  static void _bind_methods();

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };

  Ref<Weapon> m_CurrentWeapon { nullptr };
  Node3D* m_WeaponNode { nullptr }; // Weapon node is the actual weapon itself positioned in the weapon socket

  Array m_CurrentWeaponList; // This is the list that will have all the current weapons the player has equiped 
  Player* m_PlayerInst { nullptr };
  PlayerStateMachine* m_StateMachineInst { nullptr };

private:
  float m_WeaponBobTime = 0.0f, m_IdleWeaponBobTime = 0.0f; 

  bool m_IsEquipped; 
  Ref<PackedScene> m_LoadScene;

  GD_DEFINE_PROPERTY(Array, weaponList, Array());
  GD_DEFINE_PROPERTY(float, weapon_bob_freq, 2.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_amp, 0.09f);

  GD_DEFINE_PROPERTY(float, idle_weapon_bob_freq, 2.0f);
  GD_DEFINE_PROPERTY(float, idle_weapon_bob_amp, 0.09f);

};
