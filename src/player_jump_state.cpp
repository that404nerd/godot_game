#include "player.h"
#include "player_state.h"
#include "player_sprint_state.h"
#include "player_jump_state.h"

void PlayerJumpState::_enter(Player& player)
{
    m_IsJumpRequested = true;
    m_IsJumpOver = false;

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
    if(player.get_jump_buffer_timer()->get_time_left() > 0.0f) {
        
        m_PlayerVel.y = Globals::JUMP_VELOCITY;
        m_IsJumpRequested = false;
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
    
    Vector3 playerHorizVel = Vector3(m_PlayerVel.x, 0.0f, m_PlayerVel.z); // The horizontal velocity of the player
    if(playerHorizVel.length() > Globals::MaxAirMoveSpeed) { // Check if we are exceeding the max move speed in air
        playerHorizVel = playerHorizVel.normalized() * Globals::MaxAirMoveSpeed;
    }

    if(m_PlayerVel.y <= 0.0f) {
        m_CurrentSubState = SubStates::Falling;
        m_IsJumpOver = true;
    }

    // Set final velocity
    m_PlayerVel.x = playerHorizVel.x;
    m_PlayerVel.z = playerHorizVel.z;
    player.set_velocity(m_PlayerVel);
}