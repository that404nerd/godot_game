#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>

#include "player_state.h"

using namespace godot;

class PlayerJumpState : public PlayerState {

public:

    PlayerJumpState() : PlayerState("Jumping"), m_IsJumpOver(false), m_IsJumpBuffered(false) {};

    void _enter(Player& player) override;

    PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) override { return nullptr; };
    PlayerState* _physics_update(double delta, Player& player) override;

    void _handle_ground_physics(double delta, Player& player) override;
    void _handle_air_physics(double delta, Player& player) override;

private:
    bool m_IsJumpOver;
    bool m_IsJumpBuffered;
};