#include "weapon.h"
#include "godot_cpp/variant/dictionary.hpp"

using namespace godot;

void Weapon::_bind_methods() {

  ADD_GROUP("General Weapon Properties", "");

  GD_BIND_PROPERTY(Weapon, weaponName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, totalAmmoCount, Variant::INT);
  GD_BIND_PROPERTY(Weapon, magAmmoCount, Variant::INT);
  GD_BIND_PROPERTY(Weapon, is_incremental_reload, Variant::BOOL);
  
  ADD_GROUP("Weapon Sway Values", "");
  GD_BIND_PROPERTY(Weapon, weaponSwayAngularFreq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponSwayDampingRatio, Variant::FLOAT);

  GD_BIND_PROPERTY(Weapon, weaponVerticalPush, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponVerticalAngFreq, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponVerticalDampingRatio, Variant::FLOAT);

  ADD_GROUP("Weapon Animation Properties", "");
  GD_BIND_PROPERTY(Weapon, weapon_equip_anim_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_equip_anim_blend, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponEquipAnimName, Variant::STRING);

  GD_BIND_PROPERTY(Weapon, weapon_unequip_anim_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_unequip_anim_blend, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponUnequipAnimName, Variant::STRING);

  GD_BIND_PROPERTY(Weapon, weapon_shoot_anim_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_shoot_anim_blend, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponShootingAnimName, Variant::STRING);

  GD_BIND_PROPERTY(Weapon, weapon_reload_start_anim_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_reload_start_anim_blend, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponReloadStartAnimName, Variant::STRING);

  GD_BIND_PROPERTY(Weapon, weapon_reload_anim_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_reload_anim_blend, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponReloadAnimName, Variant::STRING);

  GD_BIND_PROPERTY(Weapon, weapon_reload_end_anim_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weapon_reload_end_anim_blend, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, weaponReloadEndAnimName, Variant::STRING);

  ADD_GROUP("Weapon Shoot Values", "");
  BIND_ENUM_CONSTANT(AUTO);
  BIND_ENUM_CONSTANT(MANUAL);
  BIND_ENUM_CONSTANT(BOTH);
  GD_BIND_ENUM(Weapon, weapon_type, "Manual,Auto,Both");

  GD_BIND_CUSTOM_PROPERTY(Weapon, weaponDecalResource, Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE);

  GD_BIND_PROPERTY(Weapon, shoot_buffer_time, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, hold_max_time, Variant::FLOAT);

  ADD_GROUP("Weapon Recoil Values", "");
  GD_BIND_PROPERTY(Weapon, recoil_vector, Variant::VECTOR3);
  GD_BIND_PROPERTY(Weapon, weaponRecoilResetMultiplier, Variant::FLOAT);

  ADD_GROUP("Weapon Reload Properties", "");
  GD_BIND_PROPERTY(Weapon, weaponReloadRootBoneName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, magEnteredTimestamp, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, reloadShakeSpeedMultiplier, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, reloadShakeResetMultiplier, Variant::FLOAT);

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

void Weapon::_get_property_list(List<PropertyInfo> *p_list) const 
{
  p_list->push_back(PropertyInfo(Variant::INT, "state_enum", PROPERTY_HINT_ENUM, "IDLE,WALK,RUN"));
  if(m_CurrentState == States::IDLE)
    p_list->push_back(PropertyInfo(Variant::FLOAT, "test_float"));
}

bool Weapon::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name == "state_enum") {
		m_CurrentState = p_value;
    
    if(m_CurrentState == 0)
      m_CurrentState = States::IDLE;
    else if(m_CurrentState == 1)
      m_CurrentState = States::WALK;
    else
      m_CurrentState = States::RUN;

    notify_property_list_changed();
		return true;
	}

  if(name == "test_float")
  {
    test_float = p_value; 
    return true;
  }

	return false;
}

bool Weapon::_get(const StringName &p_name, Variant &r_ret) const {
  String name = p_name;

  if (name == "state_enum") {
    if(m_CurrentState == 0)
    {
      r_ret = static_cast<int>(States::IDLE);
    }
    else if(m_CurrentState == 1)
      r_ret = static_cast<int>(States::RUN);
    else 
      r_ret = static_cast<int>(States::WALK);

    return true;
  }

  if(name == "test_float")
  {
    r_ret = test_float;
    return true;
  }

  return false;
}
