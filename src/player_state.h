#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/project_settings.hpp>

/*
    Basically there are 2 sets of states in the entire game. States like sprint, jump, crouch. You get it, the base states.
    What about idle, sliding, dashing etc...? These are stored as bits in this class and for now (as of writing this) it's gonna be managed using a set and get functions.

    * Adding a state: |=
    * Removing a state: &= ~
    * Overwriting / replacing entirely: =

    21-09-2025: Base states/Parent states work fine. Need to get sub-states working.
*/

#define BIT(x) 1 << x

class Player;

using namespace godot;

class PlayerState {
public:
    // All the other states (sub-states)
    enum class SubStates {
        Idle = BIT(0),
        Falling = BIT(1),
        Dash = BIT(2),

        NONE = -1
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
    SubStates get_current_substate() { return m_CurrentSubState; }

private:
    std::string m_StateName;
    
    protected:
    Vector3 m_WishDir;
    Vector3 m_PlayerVel;
    Vector2 m_InputDir;
    
    float m_MoveSpeed;
    SubStates m_CurrentSubState;

};

inline PlayerState::SubStates operator|(PlayerState::SubStates a, PlayerState::SubStates b) {
    return static_cast<PlayerState::SubStates>(static_cast<int>(a) | static_cast<int>(b));
}

inline PlayerState::SubStates operator&(PlayerState::SubStates a, PlayerState::SubStates b) {
    return static_cast<PlayerState::SubStates>(static_cast<int>(a) & static_cast<int>(b));
}

inline PlayerState::SubStates operator~(PlayerState::SubStates a) {
    return static_cast<PlayerState::SubStates>(~static_cast<int>(a));
}

inline PlayerState::SubStates& operator&=(PlayerState::SubStates& a, PlayerState::SubStates b) {
    a = a & b;
    return a;
}

inline PlayerState::SubStates& operator|=(PlayerState::SubStates& a, PlayerState::SubStates b) {
    a = a | b;
    return a;
}