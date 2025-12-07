#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/templates/a_hash_map.hpp>
#include <godot_cpp/variant/string.hpp>

#include "player_state.h"
#include "globals.h"

using namespace godot;

class PlayerStateMachine : public Node {
    GDCLASS(PlayerStateMachine, Node);

public:
    PlayerStateMachine();
    void _ready() override;

    void _unhandled_input(const Ref<InputEvent>& event) override;
    void _physics_process(double delta) override;

    void _change_state(const String& stateName);
    String get_current_state();

    ~PlayerStateMachine();
    
protected:
    static void _bind_methods();
    
private:
    GD_DEFINE_PROPERTY(PlayerState*, initial_state, nullptr); 

    PlayerState* m_CurrentState;
    AHashMap<String, PlayerState*> m_States;
};
