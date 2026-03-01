#pragma once

#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>

#include "globals.h"

using namespace godot;

class Weapon : public Resource {
  GDCLASS(Weapon, Resource)

public:
  Weapon();

  ~Weapon();
protected: 
  static void _bind_methods();

private:

  GD_DEFINE_PROPERTY(String, weaponName, "");
  GD_DEFINE_PROPERTY(String, weaponEquipAnimName, "");
  GD_DEFINE_PROPERTY(String, weaponShootingAnimName, "");
  GD_DEFINE_PROPERTY(String, weaponReloadAnimName, "");
  GD_DEFINE_PROPERTY(String, weaponUnequipAnimName, "");

  GD_DEFINE_PROPERTY(int, ammoCount, 0);

  GD_DEFINE_PROPERTY(float, idle_weapon_bob_freq, 0.0f);
  GD_DEFINE_PROPERTY(float, idle_weapon_bob_amp, 0.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_freq, 0.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_amp, 0.0f);

  GD_DEFINE_PROPERTY(float, gun_range, 0.0f);

  GD_DEFINE_PROPERTY(double, weapon_sway_mult, 0.002f);
  GD_DEFINE_PROPERTY(float, weapon_sway_reset, 15.0f);
  GD_DEFINE_PROPERTY(float, idle_weapon_bob_smooth_val, 2.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_smooth_val, 1.5f);

  GD_DEFINE_PROPERTY(Node3D*, weaponNode, nullptr);
};
