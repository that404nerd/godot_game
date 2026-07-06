#pragma once

#include "godot_cpp/core/print_string.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/math.hpp>

#include <magic_enum/magic_enum.hpp>

#include <iostream>
#include <functional>
#include <array>
#include <unordered_map>

using namespace godot;

/* 
  Credit to Withaust for macro (defining each one by default is a pain in the ass)
  The getter, setter functions will not match the conventions I'm using for other functions
*/
#define _CONCAT(x, y) #x "" #y
#define _TOKEN_PASTE(x, y) x##y
#define _CAT(x, y) _TOKEN_PASTE(x, y)
#define GD_CLASS(p_class, p_inherits) \
private:                              \
    typedef p_inherits Super;         \
    typedef p_class CurrentClass;     \
    GDCLASS(p_class, p_inherits);     \
    struct _CAT(__semicolon_place, __LINE__) // Forces semicolon use

#define GD_DEFINE_PROPERTY(p_type, p_name, p_default_value) \
private:                                                 \
    p_type p_name = p_default_value;                     \
public:                                                  \
    void set_##p_name(p_type value) {                    \
        p_name = value;                                  \
    }                                                    \
    p_type get_##p_name() {                              \
        return p_name;                                   \
    }                                                    \
    struct _CAT(__semicolon_place, __LINE__)

#define GD_DEFINE_COND_FUNCS() \
protected:                                                \
    void _get_property_list(List<PropertyInfo> *p_list);  \
    bool _set(const StringName &p_name, const Variant &p_value); \
    bool _get(const StringName &p_name, Variant &r_ret); \


// For binding a general property
#define GD_BIND_PROPERTY(p_class, p_name, p_type) \
        ClassDB::bind_method(D_METHOD("get_" #p_name), &p_class::get_##p_name); \
        ClassDB::bind_method(D_METHOD("set_" #p_name, "p_" #p_name), &p_class::set_##p_name); \
        ADD_PROPERTY(PropertyInfo(p_type, #p_name), "set_" #p_name, "get_" #p_name);

// For binding a property like an Array, Nodes etc...
#define GD_BIND_CUSTOM_PROPERTY(p_class, p_name, p_type, p_property_type) \
        ClassDB::bind_method(D_METHOD("get_"#p_name), &p_class::get_##p_name); \
        ClassDB::bind_method(D_METHOD("set_"#p_name, "p_"#p_name), &p_class::set_##p_name); \
        ADD_PROPERTY(PropertyInfo(p_type, #p_name, p_property_type), "set_" #p_name, "get_" #p_name);

#define GD_BIND_ENUM(p_class, p_name, p_enum_values) \
        ClassDB::bind_method(D_METHOD("get_" #p_name), &p_class::get_##p_name); \
        ClassDB::bind_method(D_METHOD("set_" #p_name, "p_" #p_name), &p_class::set_##p_name); \
        ADD_PROPERTY(PropertyInfo(Variant::INT, #p_name, PROPERTY_HINT_ENUM, p_enum_values), \
                    "set_"#p_name, "get_"#p_name);



namespace Utils {

  struct PropertyParams {
    StringName PropertyName;
    Variant::Type VariantType;
    PropertyHint PropHint;
    StringName EnumValues;
  };

  inline void add_property_cond(List<PropertyInfo> *p_list, const PropertyParams& propertyParams, std::function<bool()> condition=[](){ return true; })
  {
    bool result = condition();

    if(result)
    {
      if(propertyParams.PropHint != PROPERTY_HINT_NONE && propertyParams.EnumValues.is_empty())
      {
        p_list->push_back(PropertyInfo(propertyParams.VariantType, propertyParams.PropertyName, propertyParams.PropHint));
      }
      else if(propertyParams.PropHint == PROPERTY_HINT_ENUM && !propertyParams.EnumValues.is_empty())
      {
        p_list->push_back(PropertyInfo(propertyParams.VariantType, propertyParams.PropertyName, godot::PROPERTY_HINT_ENUM, propertyParams.EnumValues));
      } else
      {
        p_list->push_back(PropertyInfo(propertyParams.VariantType, propertyParams.PropertyName));
      }
    }

  }
  
  template <typename T>
  inline bool set_properties(const StringName& p_name, const StringName& targetPropName, const Variant& p_value, T& propVariable)
  {
    if(p_name == targetPropName)
    {
      propVariable = p_value;
      return true;
    }

    return false;
  }

  template <typename EnumType, typename T>
  inline bool set_properties_enum(const StringName& p_name, const StringName& targetPropName, const Variant& p_value, T& propVariable)
  {
    int enumIntValue = static_cast<int>(propVariable);

    constexpr auto enumArray = magic_enum::enum_values<EnumType>();
    constexpr std::size_t enumCount = magic_enum::enum_count<EnumType>();

    if (p_name == targetPropName) {
      enumIntValue = p_value;

      if((enumIntValue > static_cast<int>(enumCount) - 1)) // Subtract 1 because the enumCount doesn't start from 0 instead starts from 1
      {
        print_error("The selected enum is missing in the enum class.");
        return false;
      }

      propVariable = enumArray[p_value];
      return true;
    }


    return false;
  }

  template <typename T>
  inline bool get_property_enum(const StringName& p_name, const StringName& targetPropName, Variant& r_ret, T& propVariable)
  {
    if (p_name == targetPropName) {
      r_ret = static_cast<int>(propVariable);

      return true;
    }


    return false;
  }

  template <typename T>
  inline bool get_property(const StringName& p_name, const StringName& targetPropName, Variant& r_ret, T& propVariable)
  {
    if(p_name == targetPropName)  {
      r_ret = propVariable;
      return true;
    }

    return false;
  }
  
  // Exponential decay function
  template <typename T>
  inline T exp_decay(T a, T b, float decay, double dt)
  {
    return b + (a - b) * Math::exp(-decay * dt);
  }
};
