#pragma once

#include <godot_cpp/godot.hpp>

namespace Globals {

    
    inline float CrouchSpeed = 5.0f;
    inline float JumpSpeed = 12.0f; 
    inline float SprintSpeed = 15.0f;
    
    // inline float SlideJumpAngle = 40.0f;
    // inline float DashSpeed = 35.0f;
    // inline float CrouchSpeed = 2.0f;
    // inline float SlideSpeed = 10.0f;
    // inline float SlideJumpBoost = 15.0f;

    inline float SideTiltAngle = 5.0f;

    inline float MaxAirMoveSpeed = 12.0f; // Max speed the player can move in the air
    inline float MaxAirAccel = 10.0f; // Determines how fast the player can change the direction mid-air

    inline float MouseSensitivity = 0.004f;

    inline const float MAX_JUMP_BUFFER_TIME = 0.2f;
    inline const float HEADBOB_MOVE_AMOUNT = 0.05f;
    inline const float HEADBOB_FREQUENCY = 1.2f;
    inline const float DOWN_GRAVITY = 10.0f;
    inline const int MAX_JUMPS = 1;
};