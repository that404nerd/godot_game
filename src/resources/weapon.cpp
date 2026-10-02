#include "weapon.h"

using namespace godot;

void Weapon::_bind_methods() {

  ADD_GROUP("General Weapon Properties", "");

  GD_BIND_PROPERTY(Weapon, weaponName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, totalAmmoCount, Variant::INT);
  GD_BIND_PROPERTY(Weapon, magAmmoCount, Variant::INT);
  GD_BIND_PROPERTY(Weapon, is_incremental_reload, Variant::BOOL);
  GD_BIND_PROPERTY(Weapon, weaponFOV, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponZClipScale, Variant::FLOAT);
  
  ADD_GROUP("Weapon Sway Values", "");
  GD_BIND_PROPERTY(Weapon, weaponSwayAngularFreq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponSwayDampingRatio, Variant::FLOAT);

  GD_BIND_PROPERTY(Weapon, weaponVerticalPush, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponVerticalAngFreq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponVerticalDampingRatio, Variant::FLOAT);

  ADD_GROUP("Weapon Shoot Values", "");
  BIND_ENUM_CONSTANT(AUTO);
  BIND_ENUM_CONSTANT(MANUAL);
  BIND_ENUM_CONSTANT(BOTH);
  GD_BIND_ENUM(Weapon, weapon_type, "Manual,Auto,Both");

  // GD_BIND_CUSTOM_PROPERTY(Weapon, PackedScene, weaponRecoilPatternResource, Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(Weapon, PackedScene, weaponDecalResource, Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE);

  GD_BIND_PROPERTY(Weapon, time_between_shots, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, noOfProjectilesAtSameTime, Variant::INT);
  GD_BIND_PROPERTY(Weapon, hold_max_time, Variant::FLOAT);

  ADD_GROUP("Weapon Slide Tilt Values", "");
  GD_BIND_PROPERTY(Weapon, slide_armature_tilt_rot, Variant::VECTOR3);
  GD_BIND_PROPERTY(Weapon, slide_armature_dip, Variant::VECTOR3);
  
  GD_BIND_PROPERTY(Weapon, slide_armature_dip_reset_timer, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, slide_armature_dip_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, slide_armature_dip_reset_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, slide_armature_dip_reset_damping_ratio, Variant::FLOAT);

  GD_BIND_PROPERTY(Weapon, slide_armature_tilt_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, slide_armature_tilt_damping_ratio, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, slide_armature_tilt_end_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, slide_armature_tilt_end_damping_ratio, Variant::FLOAT);

  ADD_GROUP("Weapon Jump Tilt Values", "");
  GD_BIND_PROPERTY(Weapon, jump_armature_weapon_rise_pos, Variant::VECTOR3);
  GD_BIND_PROPERTY(Weapon, jump_armature_weapon_rot, Variant::VECTOR3);

  GD_BIND_PROPERTY(Weapon, jump_armature_weapon_rise_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, jump_armature_weapon_rise_damping_ratio, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, jump_armature_weapon_rot_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, jump_armature_weapon_rot_damping_ratio, Variant::FLOAT);

  ADD_GROUP("Weapon Recoil Values", "");
  GD_BIND_PROPERTY(Weapon, recoilMultiplier, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, recoilVector, Variant::VECTOR3);
  GD_BIND_PROPERTY(Weapon, recoil_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, recoil_damping_ratio, Variant::FLOAT);
  
  ADD_GROUP("Weapon Reload Properties", "");
  GD_BIND_PROPERTY(Weapon, weaponReloadRootBoneName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, auto_reload, Variant::BOOL);
  GD_BIND_PROPERTY(Weapon, reloadShakeResetMultiplier, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, magEnteredTimestamp, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, reloadShakeSpeedMultiplier, Variant::FLOAT);

  ADD_GROUP("Weapon Bob And Range", "");
  GD_BIND_PROPERTY(Weapon, idle_weapon_bob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, idle_weapon_bob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_bob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_bob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, gun_range, Variant::FLOAT);

  ADD_GROUP("Weapon Bob Multipliers and Smoothing Properties", "");
  GD_BIND_PROPERTY(Weapon, idle_weapon_bob_smooth_val, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_bob_smooth_val, Variant::FLOAT);
}