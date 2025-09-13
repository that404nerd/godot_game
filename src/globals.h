#pragma once

#include <godot_cpp/godot.hpp>

namespace Globals {

    inline const float CrouchSpeed = 7.0f;
    inline const float JumpSpeed = 10.0f; 
    inline const float SprintSpeed = 15.0f;
    
    inline const float DashSpeed = 35.0f;
    inline const float SideTiltAngle = 5.0f;
    // inline float SlideJumpAngle = 40.0f;
    // inline float SlideSpeed = 10.0f;
    // inline float SlideJumpBoost = 15.0f;

    inline const float MaxAirMoveSpeed = 12.0f; // Max speed the player can move in the air
    inline const float MaxAirAccel = 10.0f; // Determines how fast the player can change the direction mid-air

    inline const float MouseSensitivity = 0.004f;

    inline const float MAX_JUMP_BUFFER_TIME = 0.2f;
    inline const float HEADBOB_MOVE_AMOUNT = 0.05f;
    inline const float HEADBOB_FREQUENCY = 1.2f;
    inline const float DOWN_GRAVITY = 18.0f;

    // Friction & Bhop variables 
    inline const float GroundAccel = 14.0f;
    inline const float GroundDecel = 10.0f;
    inline const float GroundFriction = 6.0f;
};