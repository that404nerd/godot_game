#pragma once

#include <godot_cpp/classes/input.hpp>

using namespace godot;

class PlayerSprintState : public PlayerState {
public:

    PlayerSprintState() : PlayerState("Sprint") {};

    void _enter(Player& player) override;
    PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) override;
    PlayerState* _physics_update(double delta, Player& player) override { return nullptr; };

    void _handle_ground_physics(double delta, Player& player) override;
    void _handle_air_physics(double delta, Player& player) override;

    void headbob_effect(double delta, Player& player);
    virtual ~PlayerSprintState() {};

private:
    Vector3 m_PlayerVel;

    float m_HeadBobTime = 0.0f;
    bool m_JumpRequested;
};