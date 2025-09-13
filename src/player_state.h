#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/project_settings.hpp>

class Player;

using namespace godot;

class PlayerState {
public:
    PlayerState(const std::string& stateID) : m_StateID(stateID) {};

    virtual void _enter(Player& player) = 0;

    virtual PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) = 0; // This will handle the key/input based transitions
    virtual PlayerState* _physics_update(double delta, Player& player) = 0; // This will handle all the physics based transitions

    virtual void _handle_air_physics(double delta, Player& player) {};
    virtual void _handle_ground_physics(double delta, Player& player) {};
    virtual ~PlayerState() {};

    void _update(double delta, Player& player);
    

public:
    std::string get_state_name() { return m_StateID; }

private:
    std::string m_StateID; // Debug name
    
protected:
    Vector3 m_WishDir;
    Vector3 m_PlayerVel;
    Vector2 m_InputDir;

    float m_MoveSpeed;
};