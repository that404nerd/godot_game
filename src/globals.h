#pragma once

#include <godot_cpp/godot.hpp>

namespace Globals {

    
    inline float WalkSpeed = 8.0f;
    inline float SprintSpeed = 10.0f;
    inline float JumpSpeed = 5.0f; 
    
    // inline float SlideJumpAngle = 40.0f;
    // inline float DashSpeed = 35.0f;
    // inline float CrouchSpeed = 2.0f;
    // inline float SlideSpeed = 10.0f;
    // inline float SlideJumpBoost = 15.0f;

    inline float SideTiltAngle = 20.0f;

    inline float MaxAirMoveSpeed = 8.0f; // Max speed the player can move in the air
    inline float MaxAirAccel = 10.0f; // Determines how fast the player can change the direction mid-air

    inline float MouseSensitivity = 0.004f;

    inline const float MAX_JUMP_BUFFER_TIME = 0.2f;
    inline const float HEADBOB_MOVE_AMOUNT = 0.05f;
    inline const float HEADBOB_FREQUENCY = 1.8f;
};