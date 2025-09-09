#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/timer.hpp>

class Player;

using namespace godot;

class PlayerState {
public:
    PlayerState(const String& stateName) : m_StateName(stateName) {};

    virtual void _enter(Player& player) = 0;
    virtual PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) = 0;

    virtual void _handle_air_physics(double delta, Player& player) {};
    virtual void _handle_ground_physics(double delta, Player& player) {};
 
    void _update(double delta, Player& player);

    virtual ~PlayerState() {};

public:
    String get_state_name() { return m_StateName; }

private:
    String m_StateName; // Debug

protected:
    Vector3 m_WishDir;
    Vector3 m_PlayerVel;
    Vector2 m_InputDir;
    
    float m_MoveSpeed;
};