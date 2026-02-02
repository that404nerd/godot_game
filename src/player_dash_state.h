#pragma once

#include "player_state.h"
#include "player_state_machine.h"
#include "player.h"

class PlayerDashState : public PlayerState {
    GDCLASS(PlayerDashState, PlayerState);

public:
    PlayerDashState() : m_PlayerInst(nullptr) {}; 

    virtual void _enter() override;
    virtual void _handle_input(const Ref<InputEvent>& event) override;
    virtual void _physics_update(double delta) override;
    virtual void _exit() override; 

protected:
    static void _bind_methods();

private:
    Player* m_PlayerInst;
    PlayerStateMachine* m_StateMachineInst;
};
