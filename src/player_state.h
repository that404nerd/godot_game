#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input_event.hpp>

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

protected:
    static void _bind_methods();

private:
    // Player* m_PlayerInst {nullptr};
};
