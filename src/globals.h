#pragma once

#include <godot_cpp/godot.hpp>

/*
    TODO: I will probably move most of the variables to the player class
*/

// credit to Withaust for macro (defining each one by default is a pain in the ass)
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
    p_type p_name##_value = p_default_value;             \
                                                         \
public:                                                  \
    void set_##p_name(const p_type value) {              \
        p_name##_value = value;                          \
    }                                                    \
    p_type get_##p_name() const {                        \
        return p_name##_value;                           \
    }                                                    \
    struct _CAT(__semicolon_place, __LINE__)

#define GD_BIND_PROPERTY(p_class, p_name, p_type) \
        ClassDB::bind_method(D_METHOD("get_" #p_name), &p_class::get_##p_name); \
        ClassDB::bind_method(D_METHOD("set_" #p_name, "p_" #p_name), &p_class::set_##p_name); \
        ADD_PROPERTY(PropertyInfo(p_type, #p_name), "set_" #p_name, "get_" #p_name);

namespace Globals {

    inline const float CrouchSpeed = 3.0f;
    inline const float WalkSpeed = 7.0f;
    inline const float SprintSpeed = 10.0f;

    inline const float CROUCH_TRANSLATE = 0.5f;
    inline const float LERP_CONSTANT = 5.0f;
    
    inline const float DashSpeed = 35.0f;
    inline const float SideTiltAngle = 5.0f;
    // inline float SlideJumpAngle = 40.0f;
    // inline float SlideSpeed = 10.0f;

    inline const float JUMP_HEIGHT = 4.0f;
    inline const float TIME_TO_PEAK = 2.8f, TIME_TO_DESCENT = 2.8f;

    inline const float JUMP_VELOCITY = ((-2.0f * JUMP_HEIGHT) / TIME_TO_PEAK) * -1.0f;
    inline const float JUMP_GRAVITY = ((-2.0f * JUMP_HEIGHT) / TIME_TO_PEAK * TIME_TO_PEAK) * -1.0f;
    inline const float FALL_GRAVITY = ((-2.0f * JUMP_HEIGHT) / TIME_TO_DESCENT * TIME_TO_DESCENT) * -1.0f;

    inline const float MaxAirMoveSpeed = 8.0f; // Max speed the player can move in the air
    inline const float MaxAirAccel = 8.0f; // Determines how fast the player can change the direction mid-air

    inline const float MouseSensitivity = 0.004f;

    inline const float HEADBOB_MOVE_AMOUNT = 0.09f;
    inline const float HEADBOB_FREQUENCY = 1.1f;

    // Friction & Bhop variables 
    inline const float GroundAccel = 15.0f;
    inline const float GroundDecel = 10.0f;
    inline const float GroundFriction = 8.0f;
};