#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "globals.h"

using namespace godot;

class Weapon : public Resource {
  GDCLASS(Weapon, Resource)
  
protected:
  static void _bind_methods();

  void _get_property_list(List<PropertyInfo> *p_list) const;
  bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;

public:

  enum WeaponType
  {
    MANUAL, AUTO, BOTH, NONE = -1
  };


  enum States { IDLE, WALK, RUN };

private:
  GD_DEFINE_PROPERTY(String, weaponName, "");
  GD_DEFINE_PROPERTY(String, weaponReloadRootBoneName, "");

  GD_DEFINE_PROPERTY(float, weapon_equip_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_equip_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponEquipAnimName, "");

  GD_DEFINE_PROPERTY(float, weapon_shoot_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_shoot_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponShootingAnimName, "");

  GD_DEFINE_PROPERTY(float, weapon_reload_start_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_reload_start_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponReloadStartAnimName, "");

  GD_DEFINE_PROPERTY(float, weapon_reload_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_reload_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponReloadAnimName, "");

  GD_DEFINE_PROPERTY(float, weapon_reload_end_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_reload_end_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponReloadEndAnimName, "");

  GD_DEFINE_PROPERTY(float, weapon_unequip_anim_speed, 1.0f);
  GD_DEFINE_PROPERTY(float, weapon_unequip_anim_blend, -1.0f);
  GD_DEFINE_PROPERTY(String, weaponUnequipAnimName, "");

  GD_DEFINE_PROPERTY(float, hold_max_time, 0.5f);

  GD_DEFINE_PROPERTY(int, totalAmmoCount, 0); 
  GD_DEFINE_PROPERTY(int, magAmmoCount, 0); 
  GD_DEFINE_PROPERTY(bool, is_incremental_reload, false); 

  GD_DEFINE_PROPERTY(float, magEnteredTimestamp, 0.0f); 
  GD_DEFINE_PROPERTY(float, shoot_buffer_time, 0.0f);
  GD_DEFINE_PROPERTY(float, reloadShakeSpeedMultiplier, 1.0f); 
  GD_DEFINE_PROPERTY(float, reloadShakeResetMultiplier, 10.0f); 

  GD_DEFINE_PROPERTY(Vector3, recoil_vector, Vector3(0.0f, 0.0f, 0.0f)); 
  GD_DEFINE_PROPERTY(float, weaponRecoilResetMultiplier, 7.0f); 

  GD_DEFINE_PROPERTY(float, gun_range, 0.0f);
  
  GD_DEFINE_PROPERTY(WeaponType, weapon_type, WeaponType::MANUAL);

  GD_DEFINE_PROPERTY(float, weaponSwayAngularFreq, 0.0f);
  GD_DEFINE_PROPERTY(float, weaponSwayDampingRatio, 0.0f);
  GD_DEFINE_PROPERTY(float, weaponVerticalAngFreq, 0.0f);
  GD_DEFINE_PROPERTY(float, weaponVerticalDampingRatio, 0.0f);
  GD_DEFINE_PROPERTY(float, weaponVerticalPush, 1.0f);

  
  GD_DEFINE_PROPERTY(float, idle_weapon_bob_freq, 0.0f);
  GD_DEFINE_PROPERTY(float, idle_weapon_bob_amp, 0.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_freq, 0.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_amp, 0.0f);

  GD_DEFINE_PROPERTY(float, idle_weapon_bob_smooth_val, 2.0f);
  GD_DEFINE_PROPERTY(float, weapon_bob_smooth_val, 1.5f);

private:
  int m_CurrentState;
  float test_float { 0.0f };
};

VARIANT_ENUM_CAST(Weapon::WeaponType);
