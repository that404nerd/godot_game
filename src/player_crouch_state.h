#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

class PlayerCrouchState : public PlayerState {
public:

    PlayerCrouchState() : PlayerState("Crouch") {};

    void _enter(Player& player) override;

    PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) override;
    PlayerState* _physics_update(double delta, Player& player) override { return nullptr; };

    void _handle_ground_physics(double delta, Player& player) override;
    void _handle_air_physics(double delta, Player& player) override;

private:
    bool m_IsStanding;
    Vector3 m_HeadPosition;
};