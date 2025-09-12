#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input.hpp>

#include "player_crouch_state.h"
#include "player_sprint_state.h"

void PlayerCrouchState::_enter(Player& player)
{
    
}

PlayerState* PlayerCrouchState::_handle_input(const Ref<InputEvent>& event, Player& player)
{
    if(Input::get_singleton()->is_action_just_released("crouch")) {
        return memnew(PlayerSprintState);
    }

   return nullptr;
}

void PlayerCrouchState::_handle_ground_physics(double delta, Player& player)
{
}

void PlayerCrouchState::_handle_air_physics(double delta, Player& player)
{

}