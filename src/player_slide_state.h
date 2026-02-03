#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/physics_server3d.hpp>

#include "player_state.h"
#include "player_state_machine.h"
#include "player.h"
#include "globals.h"

class PlayerSlideState : public PlayerState {
    GDCLASS(PlayerSlideState, PlayerState);

public:
    PlayerSlideState() {}; 

    virtual void _enter() override;
    virtual void _handle_input(const Ref<InputEvent>& event) override;
    virtual void _physics_update(double delta) override;

    void _on_slide_finished();

    virtual void _exit() override;

protected:
    static void _bind_methods();

private:
    float m_FinalPos { 0.0f }, m_SlideTimer { 0.0f };
    Vector3 m_OriginalHeadPosition { Vector3(0.0f, 0.0f, 0.0f) };
    Player* m_PlayerInst { nullptr };

    Vector3 m_SlideVector { Vector3(0.0f, 0.0f, 0.0f) };

    PlayerStateMachine* m_StateMachineInst { nullptr };
    Ref<Tween> m_SlideTween;
    Camera3D* m_PlayerCamInst;
};
