#pragma once

#include "player_state.h"
#include "player_state_machine.h"
#include "player.h"

#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/callback_tweener.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

class PlayerCrouchState : public PlayerState {
    GDCLASS(PlayerCrouchState, PlayerState);

public:
    PlayerCrouchState() : m_CurrentHeadPosition(Vector3(0.0f, 0.0f, 0.0f)), m_SlideVector(Vector3(0.0f, 0.0f, 0.0f)), m_PlayerInst(nullptr), m_CrouchTween(nullptr) {}; 

    virtual void _enter() override;
    virtual void _handle_input(const Ref<InputEvent>& event) override;
    virtual void _physics_update(double delta) override;
    virtual void _exit() override; 

    void _on_crouch_finished();

protected:
    static void _bind_methods();

private:
    float m_FinalPos, m_SlideTimer;

    Vector3 m_OriginalHeadPosition, m_CurrentHeadPosition, m_SlideVector;
    Player* m_PlayerInst;
    Ref<Tween> m_CrouchTween;
    PlayerStateMachine* m_StateMachineInst;
};