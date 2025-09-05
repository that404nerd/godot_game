#pragma once

#include <godot_cpp/godot.hpp>

#include "player_state.h"

class PlayerSprintState;

using namespace godot;

class PlayerCrouchState : public PlayerState {
public:
    void _enter(Player& player) override;
    PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) override;
    void _update(double delta, Player& player) override;
    
};