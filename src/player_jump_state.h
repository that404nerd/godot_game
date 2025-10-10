#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

class PlayerJumpState : public PlayerState {

public:

    PlayerJumpState() : PlayerState("Jumping") {};

    void _enter(Player& player) override;

    PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) { return nullptr; };
    PlayerState* _physics_update(double delta, Player& player) override;

    void _handle_ground_physics(double delta, Player& player) override;
    void _handle_air_physics(double delta, Player& player) override;

private:
    bool m_IsJumpRequested;
    bool m_IsJumpOver;
};