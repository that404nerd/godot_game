#pragma once

/* The manager itself is a singleton to allow only a single instance in the entire game */

#include <godot_cpp/godot.hpp>
#include <iterator>

#include "player_state.h"
#include "player.h"

class FStateManager {
public:

    static FStateManager& GetStateManagerInstance()
    {
        static FStateManager* instance = memnew(FStateManager);
        return *instance;
    }

    // Doesnt really mean the function's name but whatever.
    void _initialize_manager()
    {
        m_CurrentPlayerState = memnew(PlayerSprintState);   
    }

    void toggle_states(const Ref<InputEvent>& event, Player& player)
    {
        m_OldPlayerState = m_CurrentPlayerState;
        PlayerState* newState = m_OldPlayerState->_handle_input(event, player);
        
        if(newState == nullptr || newState == m_CurrentPlayerState) return;
        
        // Actual transition
        memdelete(m_CurrentPlayerState);
        m_CurrentPlayerState = newState;
        
        m_CurrentPlayerState->_enter(player); // Call when transitioning from one state to another (will not call the default state)
    }
    
    void _update(double delta, Player& player)
    {
        m_CurrentPlayerState->_update(delta, player);
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

        // print_line(m_PlayerStates.size());
        
        // for(auto it = m_PlayerStates.begin(); it != m_PlayerStates.end(); ++it) {
        //     if(it != m_PlayerStates.begin()) {
        //         auto prev = *std::prev(it - 1);
        //         print_line("Previous state is: ", prev.first.c_str()); 
        //     }
        // }

        // std::unordered_map<std::string, PlayerState*>::iterator prevIt = m_PlayerStates.end();

        // for(auto it = m_PlayerStates.begin(); it != m_PlayerStates.end(); ++it) {
        //     if(prevIt != m_PlayerStates.end()) {
        //         print_line("Previous state is: ", prevIt->first.c_str()); 
        //     }

        //     prevIt = it;
        // }

    }

public:
    PlayerState* get_previous_state() { return m_OldPlayerState; }
    
private:
    FStateManager() {};

private:
    std::unordered_map<std::string, PlayerState*> m_PlayerStates;

    PlayerState* m_CurrentPlayerState = nullptr;
    PlayerState* m_OldPlayerState = nullptr;
};