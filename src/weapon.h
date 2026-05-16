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
  
public:
  enum WeaponType
  {
    MANUAL, AUTO, BOTH, NONE = -1
  };

private:
  GD_DEFINE_PROPERTY(String, weaponName, "");

  GD_DEFINE_PROPERTY(float, weapon_equip_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_equip_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponEquipAnimName, "");

  GD_DEFINE_PROPERTY(float, weapon_shoot_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_shoot_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponShootingAnimName, "");

  GD_DEFINE_PROPERTY(float, weapon_reload_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_reload_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponReloadAnimName, "");

  GD_DEFINE_PROPERTY(float, weapon_unequip_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_unequip_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponUnequipAnimName, "");

  GD_DEFINE_PROPERTY(int, totalAmmoCount, 0); 
  GD_DEFINE_PROPERTY(float, gun_range, 0.0f);

  GD_DEFINE_PROPERTY(Vector3, recoil_vector, Vector3(0.0f, 0.0f, 0.0f));

  GD_DEFINE_PROPERTY(WeaponType, weapon_type, WeaponType::MANUAL);

  GD_DEFINE_PROPERTY(float, angularFreq, 0.0f);
  GD_DEFINE_PROPERTY(float, dampingRatio, 0.0f);
  GD_DEFINE_PROPERTY(float, weaponVerticalAngFreq, 0.0f);
  GD_DEFINE_PROPERTY(float, weaponVerticalDampingRatio, 0.0f);
  GD_DEFINE_PROPERTY(float, weaponVerticalPush, 1.0f);

  
  GD_DEFINE_PROPERTY(float, idle_weapon_bob_freq, 0.0f);
  GD_DEFINE_PROPERTY(float, idle_weapon_bob_amp, 0.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_freq, 0.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_amp, 0.0f);

  GD_DEFINE_PROPERTY(float, idle_weapon_bob_smooth_val, 2.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_smooth_val, 1.5f);
};

VARIANT_ENUM_CAST(Weapon::WeaponType);