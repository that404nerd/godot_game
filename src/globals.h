#pragma once

#include <godot_cpp/godot.hpp>

using namespace godot;

namespace Globals {

    inline float SlideJumpAngle = 40.0f;

    inline float WalkSpeed = 5.0f;
    inline float SprintSpeed = 10.0f;
    inline float DashSpeed = 35.0f;
    inline float JumpSpeed = 8.0f; 
    inline float CrouchSpeed = 2.0f;
    inline float Delta = 10.0f;
    inline float SlideSpeed = 10.0f;
    inline float SlideJumpBoost = 15.0f;

    inline float AirCapSpeed = 1.0;
    inline float AirAccel = 2048.0f;
    inline float AirMoveSpeed = 2000.0f;

    inline float MouseSensitivity = 0.02f;
    inline const float HEADBOB_MOVE_AMOUNT = 0.06;
    inline const float HEADBOB_FREQUENCY = 2.4;
    inline const int MAX_JUMPS = 2;
};