#pragma once

#include <godot_cpp/variant/typed_dictionary.hpp>

#include "../resources/weapon.h"

/*
  The TypedDictionary for some reason in the editor is buggy as hell and needs a lot of improvement.
   
  - First, no way to re-order the key and value pair, i dont know if this is how dictionaries work but it'd be a really helpful feature.

  - Second, if the key and values are null and I accidentally click Add Key/Value pair, it just crashes. It crashes even 
    I assign for example say a PackedScene to a Weapon Resource and click on Add Key/Value pair button.

  - Third, no way to know what the key type and value type is. It's annoying as hell to check the types constantly. 
*/
typedef TypedDictionary<Ref<Weapon>, Ref<PackedScene>> WeaponDict;

class WeaponComponent : public Node
{
  GDCLASS(WeaponComponent, Node);

public:

  void _init()
  {
    set_physics_process(false);
    set_process(false);
    print_error("[color=GREEN]Weapon Component Initialized");
  }

  void set_current_weapon(Ref<Weapon> newWeapon) { 
    m_CurrentWeapon = newWeapon; 
  }

  Ref<Weapon> get_current_weapon_data() const { 
    if(!m_CurrentWeapon.is_valid())
    {
      print_error("Get weapon: Current weapon is null!");
      return nullptr;
    }

    return m_CurrentWeapon;
  }

  void set_next_weapon(Ref<Weapon> nextWeapon) { m_NextWeapon = nextWeapon; }
  Ref<Weapon> get_next_weapon_data() const { return m_NextWeapon; }

  void set_next_weapon_name(StringName nextWeaponName) { m_NextWeaponName = nextWeaponName; }
  StringName get_next_weapon_name() { return m_NextWeaponName; }

  Array get_weapon_list() { return weapon_resource_list.keys(); }
  Array get_weapon_scene_list() { return weapon_resource_list.values(); }

protected:
  static void _bind_methods()
  {
    GD_BIND_PROPERTY(WeaponComponent, weapon_resource_list, Variant::DICTIONARY); 

    ClassDB::bind_method(D_METHOD("get_current_weapon_data"), &WeaponComponent::get_current_weapon_data);
  }

private:
  GD_DEFINE_PROPERTY(WeaponDict, weapon_resource_list, WeaponDict());

  Array m_WeaponList { Array() };

  StringName m_NextWeaponName;
  Ref<Weapon> m_CurrentWeapon { nullptr };
  Ref<Weapon> m_NextWeapon { nullptr };
};
