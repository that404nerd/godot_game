#include "player.h"
#include "player_state.h"
#include "player_sprint_state.h"
#include "player_jump_state.h"

void PlayerJumpState::_enter(Player& player)
{
    m_IsJumpRequested = true;
    m_IsJumpOver = false;
    FStateManager::GetStateManagerInstance().add_player_state(this);
}

PlayerState* PlayerJumpState::_handle_input(const Ref<InputEvent>& event, Player& player)
{
    return nullptr;
}

PlayerState* PlayerJumpState::_physics_update(double delta, Player& player)
{
    if(player.is_on_floor() && m_IsJumpOver) {
        return memnew(PlayerSprintState);
    }

    return nullptr;
}


void PlayerJumpState::_handle_ground_physics(double delta, Player& player)
{
    m_PlayerVel = player.get_velocity();

    if(player.is_on_floor() && m_IsJumpRequested) {
        m_PlayerVel.y = Globals::JumpSpeed;
        m_IsJumpRequested = false;
        m_IsJumpOver = false;
    } else if(m_PlayerVel.y > 0.0f) {
        m_IsJumpOver = true;
    }
   
    player.set_velocity(m_PlayerVel);
}

void PlayerJumpState::_handle_air_physics(double delta, Player& player)
{
    m_PlayerVel = player.get_velocity();
    
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
    
    // Set final velocity
    m_PlayerVel.x = playerHorizVel.x;
    m_PlayerVel.z = playerHorizVel.z;
    
    player.set_velocity(m_PlayerVel);

}