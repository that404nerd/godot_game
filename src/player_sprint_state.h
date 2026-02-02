#pragma once

#include "player_state.h"
#include "player_state_machine.h"
#include "player.h"

#include "globals.h"

class PlayerSprintState : public PlayerState {
    GDCLASS(PlayerSprintState, PlayerState);

public:
    PlayerSprintState() : m_HeadBobTime(0.0f) {}; 

    virtual void _enter() override;
    virtual void _handle_input(const Ref<InputEvent>& event) override;
    virtual void _physics_update(double delta) override;
    virtual void _exit() override; 

    void headbob_effect(double delta);

protected:
    static void _bind_methods();

private:
    Player* m_PlayerInst;
    PlayerStateMachine* m_StateMachineInst;

    float m_HeadBobTime;
    GD_DEFINE_PROPERTY(float, headbob_move_freq, 1.2f);
    GD_DEFINE_PROPERTY(float, headbob_move_amt, 0.07f);
};