#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

class PlayerJumpState : public PlayerState {

public:

    PlayerJumpState() : PlayerState("Jump") {};

    void _enter(Player& player) override;

    PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) override;
    void _handle_ground_physics(double delta, Player& player) override;
    void _handle_air_physics(double delta, Player& player) override;

};