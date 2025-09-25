#include "player.h"
#include "player_state.h"
#include "player_sprint_state.h"
#include "player_jump_state.h"

void PlayerJumpState::_enter(Player& player)
{
    m_IsJumpRequested = true;
    m_IsJumpOver = false;
    m_JumpBufferTimer = 0.2f;

    FStateManager::GetStateManagerInstance().add_player_state(this);
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
    m_PlayerVel = player.get_velocity();
    
    if(m_JumpBufferTimer > 0.0f) {
        m_PlayerVel.y = Globals::JumpSpeed;
        m_IsJumpRequested = false;
        m_JumpBufferTimer -= delta;
    } else {
        m_IsJumpOver = true;
    }
    
    if(m_IsJumpOver) {
        m_CurrentSubState = SubStates::Falling;
        m_JumpBufferTimer = 0.0f;
    } else {
        m_CurrentSubState = SubStates::NONE;
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

    print_line("Jump buffer timer: ", m_JumpBufferTimer);
    
    // if(Input::get_singleton()->is_action_just_pressed("dash")) {
    //     m_CurrentSubState = SubStates::Dash;
    //     m_PlayerVel.x = Math::lerp(m_PlayerVel.x, m_PlayerVel.x * Globals::AirDashSpeed, (float)delta * 5.0f);
    //     m_PlayerVel.z = Math::lerp(m_PlayerVel.z, m_PlayerVel.z * Globals::AirDashSpeed, (float)delta * 5.0f);
    // } else {
    //     m_CurrentSubState &= ~SubStates::Dash; // Look into this later...
    // }
    
    // Set final velocity
    m_PlayerVel.x = playerHorizVel.x;
    m_PlayerVel.z = playerHorizVel.z;
    player.set_velocity(m_PlayerVel);

}