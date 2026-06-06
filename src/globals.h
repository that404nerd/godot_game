#pragma once

#include "godot_cpp/core/class_db.hpp"
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/math.hpp>

#include <array>

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

  // Exponential decay function
  template <typename T>
  inline T exp_decay(T a, T b, float decay, double dt)
  {
      return b + (a - b) * Math::exp(-decay * dt);
  }
};
