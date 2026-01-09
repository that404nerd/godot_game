#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/print_string.hpp>
#include <godot_cpp/variant/string_name.hpp>

#include <cassert>

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

#define GD_BIND_PROPERTY(p_class, p_name, p_type) \
        ClassDB::bind_method(D_METHOD("get_" #p_name), &p_class::get_##p_name); \
        ClassDB::bind_method(D_METHOD("set_" #p_name, "p_" #p_name), &p_class::set_##p_name); \
        ADD_PROPERTY(PropertyInfo(p_type, #p_name), "set_" #p_name, "get_" #p_name);

// This is for assigning a value to the property
#define GD_BIND_CUSTOM_PROPERTY(p_class, p_name, p_type, p_property_type) \
        ClassDB::bind_method(D_METHOD("get_" #p_name), &p_class::get_##p_name); \
        ClassDB::bind_method(D_METHOD("set_" #p_name, "p_" #p_name), &p_class::set_##p_name); \
        ADD_PROPERTY(PropertyInfo(p_type, #p_name, p_property_type), "set_" #p_name, "get_" #p_name);


namespace Globals 
{
  enum class StateNames {
    IDLE, SPRINT, CROUCH, JUMP, DASH, FALL, SLIDE, NONE = -1  
  };

  inline StringName SetCurrentState(StateNames state)
  {
    StringName currentState;
    switch(state)
    {
      case StateNames::IDLE:
        currentState = "Idle";
        break;
      case StateNames::SPRINT:
        currentState = "Sprint";
        break;
      case StateNames::JUMP:
        currentState = "Jump";
        break;
      case StateNames::FALL:
        currentState = "Fall";
        break;
      case StateNames::DASH:
        currentState = "Dash";
        break;
      case StateNames::CROUCH:
        currentState = "Crouch";
        break;
      case StateNames::SLIDE:
        currentState = "Slide";
        break;
      default:
        assert("State Name Invalid!!");
        break;
    }

    print_line(currentState);

    return currentState;
  }

};
