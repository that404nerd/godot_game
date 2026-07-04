#include "weapon.h"
#include "godot_cpp/classes/global_constants.hpp"
#include "magic_enum/magic_enum.hpp"

using namespace godot;

void Weapon::_bind_methods() {

  ADD_GROUP("General Weapon Properties", "");

  GD_BIND_PROPERTY(Weapon, weaponName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, totalAmmoCount, Variant::INT);
  GD_BIND_PROPERTY(Weapon, magAmmoCount, Variant::INT);
  GD_BIND_PROPERTY(Weapon, is_incremental_reload, Variant::BOOL);
  GD_BIND_PROPERTY(Weapon, weaponFOV, Variant::FLOAT);
  
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

  GD_BIND_PROPERTY(Weapon, time_between_shots, Variant::FLOAT);
  GD_BIND_PROPERTY(Weapon, noOfProjectilesAtSameTime, Variant::INT);
  GD_BIND_PROPERTY(Weapon, hold_max_time, Variant::FLOAT);

  ADD_GROUP("Weapon Recoil Values", "");
  GD_BIND_PROPERTY(Weapon, recoil_vector, Variant::VECTOR3);
  GD_BIND_PROPERTY(Weapon, weaponRecoilResetMultiplier, Variant::FLOAT);

  ADD_GROUP("Weapon Reload Properties", "");
  GD_BIND_PROPERTY(Weapon, weaponReloadRootBoneName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, auto_reload, Variant::BOOL);
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

/*
* This is the function that pushes the properties into the list
*/
void Weapon::_get_property_list(List<PropertyInfo> *p_list)
{
  Utils::add_property_cond(p_list, { 
    .PropertyName="state_enum", 
    .VariantType=Variant::INT, 
    .PropHint=PROPERTY_HINT_ENUM, 
    .EnumValues="IDLE,WALK,RUN",
    .PropertyVariable=static_cast<int>(m_CurrentState)
  });

  Utils::add_property_cond(p_list, { 
    .PropertyName="test_float", 
    .VariantType=Variant::FLOAT, 
    .PropHint=PROPERTY_HINT_NONE, 
    .EnumValues="",
    .PropertyVariable=test_float
  },
  [this](){ return m_CurrentState == States::IDLE; });
  
}

/*
* The _set function takes the property name as the first parameter,
* The p_value is like a template parameter. It accepts any standard godot types. It stores the value of the property pushed into the list
* This function just sets the value to the property, we are setting a custom class's properties to the p_value
*/
bool Weapon::_set(const StringName &p_name, const Variant &p_value) {

  Utils::set_properties(p_name, p_value, test_float);

  auto states = magic_enum::enum_values<States>();

  Utils::set_properties(p_name, p_value, m_CurrentState, states);
  int state = static_cast<int>(m_CurrentState);

	// if (p_name == StringName("state_enum")) {
	// 	state = p_value;
	//
	//    if(state == 0)
	//      m_CurrentState = States::IDLE;
	//    else if(state == 1)
	//      m_CurrentState = States::WALK;
	//    else
	//      m_CurrentState = States::RUN;
	//
	//    notify_property_list_changed();
	// 	return true;
	// }

	return false;
}

bool Weapon::_get(const StringName &p_name, Variant &r_ret) 
{
  String name = p_name;

  if (name == "state_enum") {
    if(m_CurrentState == States::IDLE)
    {
      r_ret = static_cast<int>(States::IDLE);
    }
    else if(m_CurrentState == States::WALK)
      r_ret = static_cast<int>(States::WALK);
    else 
      r_ret = static_cast<int>(States::RUN);

    return true;
  }

  if(name == "test_float")
  {
    r_ret = test_float;
    return true;
  }

  return false;
}
