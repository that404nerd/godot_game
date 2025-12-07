#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "player.h"

class PlayerStateMachine;

using namespace godot;

class PlayerState : public Node {
    GDCLASS(PlayerState, Node);

public:
    PlayerState();
    virtual ~PlayerState() {};

    virtual void _enter() = 0;
    virtual void _handle_input(const Ref<InputEvent>& event) = 0;
    virtual void _physics_update(double delta) = 0;
    virtual void _exit() = 0;

    PlayerStateMachine* get_player_state_machine() { return Object::cast_to<PlayerStateMachine>(get_parent()); }
    void set_player_inst(Player* player) { m_PlayerInst = player; }
    Player* get_player_inst() { return m_PlayerInst; }

protected:
    static void _bind_methods();

private:
    Player* m_PlayerInst;
};
