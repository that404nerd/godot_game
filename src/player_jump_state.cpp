#include "globals.h"
#include "player.h"
#include "player_state.h"
#include "player_sprint_state.h"
#include "player_state_manager.h"
#include "player_jump_state.h"

void PlayerJumpState::_enter(Player& player)
{
    FStateManager::GetStateManagerInstance().add_player_state(this);

    m_PlayerVel = player.get_velocity();
}

PlayerState* PlayerJumpState::_physics_update(double delta, Player& player)
{
    // The condition checks if the player is on the floor and that the jump is over, for which we use m_IsJumpOver as a way to keep track of it.
    if(player.is_on_floor() && m_IsJumpOver) { 
        return memnew(PlayerSprintState);
    }
    
    return nullptr;
}

void PlayerJumpState::_handle_ground_physics(double delta, Player& player)
{
    player.get_jump_buffer_timer()->start();
    
    if(player.is_on_floor()) {
        m_PlayerVel.y = Globals::JUMP_VELOCITY;
    }  
    
    player.set_velocity(m_PlayerVel);
}

void PlayerJumpState::_handle_air_physics(double delta, Player& player)
{
    // Air strafing
    float currentSpeed = m_PlayerVel.dot(m_WishDir);
    float addSpeed = Globals::MaxAirMoveSpeed - currentSpeed; // how much speed we can add to the player before exceeding max move speed in air
    
    if (addSpeed > 0.0f) {
        float accel = Globals::MaxAirAccel * addSpeed;
        m_PlayerVel += m_WishDir * (accel * delta); // v = u + a * t
    }

    if (m_PlayerVel.y <= 0.0f && !player.is_on_floor()) {
        if (!FStateManager::GetStateManagerInstance().has_player_substate("Falling")) {
            FStateManager::GetStateManagerInstance().add_player_substate("Falling");
            m_IsJumpOver = true;
            print_line("Falling");
        }
    }

    if (FStateManager::GetStateManagerInstance().get_current_player_substate() == "Falling" && 
        Input::get_singleton()->is_action_just_pressed("jump") && !m_IsJumpBuffered)
    {
        m_PlayerVel.y = Globals::JUMP_VELOCITY;
        FStateManager::GetStateManagerInstance().delete_player_substate("Falling");
        m_IsJumpBuffered = true; // Prevent repeated jumps in midair
        print_line("Jump buffer!");
    }

    if (player.is_on_floor()) {
        m_IsJumpBuffered = false;
        m_IsJumpOver = true;
        if (FStateManager::GetStateManagerInstance().has_player_substate("Falling")) {
            FStateManager::GetStateManagerInstance().delete_player_substate("Falling");
        }
    }


    player.set_velocity(m_PlayerVel);
}