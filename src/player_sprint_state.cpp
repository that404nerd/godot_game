#include "player.h"
#include "player_state.h"
#include "player_sprint_state.h"
#include "player_crouch_state.h"
#include "player_jump_state.h"

void PlayerSprintState::_enter(Player& player)
{
}

PlayerState* PlayerSprintState::_handle_input(const Ref<InputEvent>& event, Player& player)
{

    print_line("Jump buffer timer left: ", player.get_jump_buffer_timer()->get_time_left());

    if(Input::get_singleton()->is_action_just_pressed("crouch")) {
        return memnew(PlayerCrouchState);
    } else if(Input::get_singleton()->is_action_just_pressed("jump")) {
        player.get_jump_buffer_timer()->start();
        return memnew(PlayerJumpState);
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