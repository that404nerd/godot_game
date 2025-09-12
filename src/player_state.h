#pragma once

#include <godot_cpp/classes/input_event.hpp>

class Player;

using namespace godot;

class PlayerState {
public:

    // Pre-defined sub-states
    enum class SubStates {
        Idle, // Sprint state
        Falling, // Jump state
        Slide // Crouch state
    };

public:
    PlayerState(const std::string& stateID) : m_StateID(stateID) {};

    virtual void _enter(Player& player) = 0;

    virtual PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) = 0;
    virtual PlayerState* _physics_update(double delta, Player& player) = 0;

    virtual void _handle_air_physics(double delta, Player& player) {};
    virtual void _handle_ground_physics(double delta, Player& player) {};
    virtual ~PlayerState() {};

    void _update(double delta, Player& player);
    

public:
    std::string get_state_name() { return m_StateID; }

    bool get_state_over() { return m_StateOver; }
    void set_state_over(bool over) { m_StateOver = over; }

private:
    std::string m_StateID; // Debug name
    bool m_StateOver; // A flag to check if the current state is over or not and is used when transitioning between states
    
protected:
    Vector3 m_WishDir;
    Vector3 m_PlayerVel;
    Vector2 m_InputDir;

    float m_MoveSpeed;
};