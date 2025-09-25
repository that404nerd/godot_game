#include "player.h"
#include "player_state.h"
#include "player_sprint_state.h"
#include "player_crouch_state.h"
#include "player_jump_state.h"

#include "player_state_manager.h"

void PlayerSprintState::_enter(Player& player)
{
    m_JumpRequested = false;
    FStateManager::GetStateManagerInstance().add_player_state(this);
}

PlayerState* PlayerSprintState::_handle_input(const Ref<InputEvent>& event, Player& player)
{
    if(Input::get_singleton()->is_action_just_pressed("crouch")) {
        return memnew(PlayerCrouchState);
    } else if(Input::get_singleton()->is_action_just_pressed("jump")) {
        // queue a jump request because input function runs only during inputs (lol)
        if(static_cast<int>(get_current_substate()) & static_cast<int>(SubStates::Falling)) {
            m_JumpRequested = true;
        } else {
            return memnew(PlayerJumpState);
        }
    }

    return nullptr;
}

void PlayerSprintState::headbob_effect(double delta, Player& player)
{
    Vector3 playerVelHeadbob = Vector3(m_PlayerVel.x, 0.0f, m_PlayerVel.z); // Use this vector so that y-vel (gravity) doesn't affect the headbob
    m_HeadBobTime += playerVelHeadbob.length() * delta;
    
    Transform3D headbobTransform = player.get_player_head()->get_transform(); // get the player's transform
    headbobTransform.origin = Vector3( // like a sine wave
        Math::cos(m_HeadBobTime * Globals::HEADBOB_FREQUENCY) * Globals::HEADBOB_MOVE_AMOUNT,
        Math::sin(m_HeadBobTime * Globals::HEADBOB_FREQUENCY) * Globals::HEADBOB_MOVE_AMOUNT,
        0.0f
    );
    player.get_player_head()->set_transform(headbobTransform);
}

void PlayerSprintState::_handle_ground_physics(double delta, Player& player)
{
    m_PlayerVel = player.get_velocity();
    
    float currentSpeedInWishDir = m_PlayerVel.dot(m_WishDir);
    float addSpeed = m_MoveSpeed - currentSpeedInWishDir;
    
    if(addSpeed > 0.0f) {
        float accel = Globals::GroundAccel * m_MoveSpeed * delta;
        accel = Math::min(accel, addSpeed);
        m_PlayerVel += accel * m_WishDir;
    } 
    
    // Friciton code
    float control = Math::max(m_PlayerVel.length(), Globals::GroundDecel); // Dont let speed to drop to zero instead to ground decl when stopping
    float drop = control * Globals::GroundFriction * delta; // how much velocity should be dropped due to friction
    float newSpeed = Math::max(m_PlayerVel.length() - drop, 0.0f); // New speed has to be subtracted from the current velocity due to friction
    
    if(m_PlayerVel.length() > 0.0f) {
        newSpeed /= m_PlayerVel.length();
    }
    
    m_PlayerVel *= newSpeed;

    if(m_PlayerVel.length() == 0) {
        m_CurrentSubState = SubStates::Idle;
    } else if(Input::get_singleton()->is_action_just_pressed("dash")) {
        m_CurrentSubState = SubStates::Dash;
        m_PlayerVel.x = Math::lerp(m_PlayerVel.x, m_PlayerVel.x * Globals::DashSpeed, (float)delta * 5.0f);
        m_PlayerVel.z = Math::lerp(m_PlayerVel.z, m_PlayerVel.z * Globals::DashSpeed, (float)delta * 5.0f);
    } else {
        m_CurrentSubState = SubStates::NONE;
    }

    if (!Math::is_equal_approx(m_InputDir.x, 0.0f) && player.is_on_floor()) {
        float targetTilt = (m_InputDir.x > 0 ? -Globals::SideTiltAngle : Globals::SideTiltAngle);
        m_PlayerTiltVector.z = Math::lerp(m_PlayerTiltVector.z, Math::deg_to_rad(targetTilt), (float)delta * 5.0f);
    } else {
        m_PlayerTiltVector.z = Math::lerp(m_PlayerTiltVector.z, 0.0f, (float)delta * 5.0f);
    }
    
    player.get_player_head()->set_rotation(m_PlayerTiltVector);
    
    headbob_effect(delta, player);
    player.set_velocity(m_PlayerVel);
}

void PlayerSprintState::_handle_air_physics(double delta, Player& player)
{
    m_PlayerVel = player.get_velocity();

    if(m_PlayerVel.y < 0.0f && !player.is_on_floor()) {
        m_CurrentSubState = SubStates::Falling;
    } else {
        m_CurrentSubState = SubStates::NONE;
    }

    if((get_current_substate() == SubStates::Falling && m_JumpRequested)) {
        m_JumpRequested = false;
        // Manually transition to the jump state
        FStateManager::GetStateManagerInstance().transition_states(player, delta, nullptr, memnew(PlayerJumpState), true);
    }

    if(get_current_substate() == SubStates::Falling) {
        // Allow air strafing during the fall state
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

}