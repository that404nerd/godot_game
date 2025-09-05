#pragma once

#include "player_state.h"

// Forward declarations to avoid circular includes
class Player;

using namespace godot;

// This class will handle both the sprint and the idle states for now.
class PlayerSprintState : public PlayerState {
public:
    void _enter(Player& player) override;
    PlayerState* _handle_input(const Ref<InputEvent>& event, Player& player) override;

    void headbob_effect(double delta, Player& player);

    void _update(double delta, Player& player) override;

    virtual ~PlayerSprintState() {};
private:
    Vector3 m_PlayerTiltVector;

    float m_MoveSpeed = 0.0f;
    float m_HeadBobTime = 0.0f;

    Vector3 m_WishDir;
    Vector3 m_PlayerVel;
    Vector2 m_InputDir;
};