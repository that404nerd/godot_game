#include "player.h"
#include "player_crouch_state.h"
#include "player_sprint_state.h"

void PlayerCrouchState::_enter(Player& player)
{
    m_HeadPosition = player.get_player_head()->get_position();
    FStateManager::GetStateManagerInstance().add_player_state(this);

    m_PlayerVel = player.get_velocity();
}

PlayerState* PlayerCrouchState::_handle_input(const Ref<InputEvent>& event, Player& player)
{
    if (Input::get_singleton()->is_action_just_pressed("crouch"))
    {
        player.get_player_crouching_collider()->set_disabled(true);
        player.get_player_standing_collider()->set_disabled(false);

        Vector3 current_position = player.get_player_head()->get_position();
        current_position.y = 2.0f; // The release of crouch is sudden (for now)
        player.get_player_head()->set_position(current_position);
        
        return memnew(PlayerSprintState);
    }

    if(Input::get_singleton()->is_action_just_pressed("jump")) {
        return memnew(PlayerJumpState);
    }

    return nullptr;
}

void PlayerCrouchState::_handle_ground_physics(double delta, Player& player)
{
    Vector3 current_position = player.get_player_head()->get_position();
    current_position.y = Math::lerp(current_position.y, m_HeadPosition.y - Globals::CROUCH_TRANSLATE, (float)delta * Globals::LERP_CONSTANT);
    player.get_player_head()->set_position(current_position);

    // Set collider states
    player.get_player_crouching_collider()->set_disabled(false);
    player.get_player_standing_collider()->set_disabled(true);

    m_PlayerVel = Globals::CrouchSpeed * m_WishDir;

    player.set_velocity(m_PlayerVel);
}

void PlayerCrouchState::_handle_air_physics(double delta, Player& player)
{
}
