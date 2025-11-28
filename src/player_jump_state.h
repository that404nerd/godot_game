#pragma once

#include "player_state.h"
#include "player_state_machine.h"
#include "player.h"

class PlayerJumpState : public PlayerState {
    GDCLASS(PlayerJumpState, PlayerState);

public:
    PlayerJumpState() : m_PlayerVel(Vector3(0.0f, 0.0f, 0.0f)), m_PlayerInst(nullptr) {}; 

    virtual void _enter() override;
    virtual void _handle_input(const Ref<InputEvent>& event) override;
    virtual void _physics_update(double delta) override;
    virtual void _exit() override; 

protected:
    static void _bind_methods();

private:
    Vector3 m_PlayerVel;
    Player* m_PlayerInst;
};