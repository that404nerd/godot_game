#include "player_state.h"
#include "player.h"

void PlayerState::_update(double delta, Player& player)
{
    m_InputDir = player.get_input_dir();
    m_WishDir = player.get_wish_dir();
    m_PlayerVel = player.get_velocity();

    // Apply the default gravity here to not have multiple states manage gravity
    if(!player.is_on_floor()) {
       m_PlayerVel.y -= get_gravity() * delta;
    }

    player.set_velocity(m_PlayerVel);

    if (player.is_on_floor())
    {
        _handle_ground_physics(delta, player);
    }
    else
    {
        _handle_air_physics(delta, player);
    }
    
}