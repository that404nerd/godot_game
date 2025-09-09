#pragma once

#include <godot_cpp/classes/input.hpp>

class Player;

using namespace godot;

// This class will handle both the sprint and the idle states (for now maybe...)
class PlayerSprintState : public PlayerState {
public:

    PlayerSprintState() : PlayerState("Sprint") {};

    void _enter(Player& player) override;
    PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) override;

    void _handle_ground_physics(double delta, Player& player) override;
    void _handle_air_physics(double delta, Player& player) override {};
    void headbob_effect(double delta, Player& player);

    virtual ~PlayerSprintState() {};
private:
    Vector3 m_PlayerTiltVector;

    float m_HeadBobTime = 0.0f;
};