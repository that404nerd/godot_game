#pragma once

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

class Player; // forward declaration to break circular dependency

using namespace godot;

class PlayerState {
public:
    virtual void _enter(Player& player) = 0;
    virtual PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) = 0;
    virtual void _update(double delta, Player& player) = 0;

    virtual ~PlayerState() {};
};