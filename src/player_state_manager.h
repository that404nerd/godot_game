#pragma once

/* The manager is a singleton to allow only a single instance of it in the game */

#include <godot_cpp/godot.hpp>

#include "player_state.h"
#include "player.h"

class FStateManager {
public:

    static FStateManager& GetStateManagerInstance()
    {
        static FStateManager* instance = memnew(FStateManager);
        return *instance;
    }

    // Function name is misleading but whatever.
    void _initialize_manager(Player& player)
    {
        m_CurrentPlayerState = memnew(PlayerSprintState);   
        m_CurrentPlayerState->_enter(player); // Call the default state's enter function
    }

    void toggle_states(const Ref<InputEvent>& event, Player& player)
    {
        PlayerState* newState = m_CurrentPlayerState->_handle_input(event, player);

        if(newState == nullptr) return;

        delete_player_state(m_CurrentPlayerState->get_state_name());
        memdelete(m_CurrentPlayerState);

        m_CurrentPlayerState = newState;
        m_CurrentPlayerState->_enter(player);
    }

    void _update(double delta, Player& player)
    {
        m_CurrentPlayerState->_update(delta, player);

        PlayerState* newState = m_CurrentPlayerState->_physics_update(delta, player);

        if(newState != nullptr)
        {
            delete_player_state(m_CurrentPlayerState->get_state_name());
            memdelete(m_CurrentPlayerState);

            m_CurrentPlayerState = newState;
            m_CurrentPlayerState->_enter(player);
        }

        if (m_CurrentPlayerState != nullptr) {
            m_CurrentPlayerState->_update(delta, player);
        }

    }


    void add_player_state(PlayerState* playerState)
    {
        if(playerState) {
            m_PlayerStates[playerState->get_state_name()] = playerState;
        }
    }
    
    void delete_player_state(const std::string& stateName)
    {
        for (auto it = m_PlayerStates.begin(); it != m_PlayerStates.end();)
        {
            if (it->first == stateName)
                it = m_PlayerStates.erase(it);
            else
                ++it;
        }
    }
    
    void print_player_states()
    {
        for(auto& state : m_PlayerStates) {
            print_line("Current player State is: ", state.first.c_str());
        }  

    }

private:
    FStateManager() {};

private:
    std::unordered_map<std::string, PlayerState*> m_PlayerStates;

    PlayerState* m_CurrentPlayerState = nullptr;
};