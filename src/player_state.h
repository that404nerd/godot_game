#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/project_settings.hpp>

#include "globals.h"

/*
    Basically there are 2 sets of states in the entire game. States like sprint, jump, crouch. You get it, the base states.
    What about idle, sliding, dashing etc...? These are stored as enums in this class and for now (as of writing this) it's gonna be managed using a set and get functions.

    21-09-2025: Base states/Parent states work fine. Need to get sub-states working.
    7-10-2025: Sub-States but are kinda shit
*/

class Player;

using namespace godot;

class PlayerState {
public:
    // All the other states (sub-states). TODO: Rework the entire sub-states into some struct or smtg
    enum class SubStates {
        Idle = 1,
        Falling = 2,
        Dash = 3,

        NONE = 0
    };

    PlayerState(const std::string& stateName) : m_StateName(stateName) {};

    virtual void _enter(Player& player) = 0;

    virtual PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) = 0; // This will handle the key/input based transitions
    virtual PlayerState* _physics_update(double delta, Player& player) = 0; // This will handle all the physics based transitions

    virtual void _handle_air_physics(double delta, Player& player) {};
    virtual void _handle_ground_physics(double delta, Player& player) {};
    virtual ~PlayerState() {};

    void _update(double delta, Player& player);
    
public:
    std::string get_state_name() { return m_StateName; }
    float get_gravity() { return m_PlayerVel.y > 0.0f ? Globals::JUMP_GRAVITY : Globals::FALL_GRAVITY; }
    SubStates get_current_substate() { return m_CurrentSubState; }

private:
    std::string m_StateName;
    
protected:
    Vector3 m_WishDir;
    Vector3 m_PlayerVel;
    Vector2 m_InputDir;
    
    SubStates m_CurrentSubState;

};