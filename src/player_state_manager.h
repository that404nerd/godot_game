#pragma once

/* The manager is a singleton to allow only a single instance of it in the whole game
   IMPORTANT: DO NOT set the m_PlayerVel in each function and instead set it in the _enter function instead to not fuck up the velocity when transitioning between states
*/

#include <godot_cpp/godot.hpp>

#include <unordered_map>
#include <vector>
#include <algorithm>

#include "godot_cpp/classes/sub_viewport.hpp"
#include "player_state.h"
#include "player.h"

class FStateManager {
public:

    static FStateManager& GetStateManagerInstance()
    {
        static FStateManager instance;
        return instance;
    }

    // Function name is misleading but whatever.
    void _initialize_manager(Player& player)
    {
        m_CurrentPlayerState = memnew(PlayerSprintState);   
        m_CurrentPlayerState->_enter(player); // Call the default state's enter function
    }

    // Handles manual and automatic transition of states (Do not call the _update function here. The _physics_update takes care of it!)
    void transition_states(Player& player, double delta, const Ref<InputEvent>& event = nullptr, PlayerState* playerState = nullptr, bool isTransition = false)
    {
        // Store old state temporarily
        PlayerState* oldState = m_CurrentPlayerState;

        if(isTransition) {
            // Manual transition: force new state
            m_CurrentPlayerState = playerState;
            m_CurrentPlayerState->_enter(player);
        } else {
            PlayerState* newState = m_CurrentPlayerState->_handle_input(event, player);
            if(newState == nullptr) return;

            m_CurrentPlayerState = newState;
            m_CurrentPlayerState->_enter(player);
        }

        // Delete the old state
        if(oldState) {
            delete_player_state(oldState->get_state_name());
            memdelete(oldState);
        }
    }


    // This will handle the physics related transitions (Not for transitioning!!!!!)
    void _physics_update(double delta, Player& player)
    {
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
        
        
        // if(static_cast<int>(m_CurrentPlayerState->get_current_substate()) != 0)
        //     print_line("Current substate: ", static_cast<uint32_t>(m_CurrentPlayerState->get_current_substate()));
        // else
        //     print_line("Current substate is null");
    }

    std::string get_current_player_state()
    {
        std::string stateName;
        for(auto& state : m_PlayerStates) {
            stateName = state.first;
        }
        
        return stateName;
    }

    std::string get_current_player_substate()
    {
        std::string stateName;
        for(auto& state : m_PlayerSubStates) {
            stateName = state;
        }
        
        return stateName;
    }
    
    void add_player_state(PlayerState* playerState)
    {
        if(playerState) {
            m_PlayerStates[playerState->get_state_name()] = playerState;
        }
    }

    void add_player_substate(const std::string& playerSubState)
    {
        if (!playerSubState.empty() && std::find(m_PlayerSubStates.begin(), m_PlayerSubStates.end(), playerSubState) == m_PlayerSubStates.end()) 
        {
            m_PlayerSubStates.emplace_back(playerSubState);
        }
    }

    bool has_player_substate(const std::string& subStateName) const
    {
        return std::find(m_PlayerSubStates.begin(), m_PlayerSubStates.end(), subStateName) != m_PlayerSubStates.end();
    }
    
    // TODO: Return the prev-state (Base, Sub-States)
    PlayerState* get_prev_state()
    {
        return nullptr;
    }
    
    
    void delete_player_substate(const std::string& subStateName)
    {
        for (auto it = m_PlayerSubStates.begin(); it != m_PlayerSubStates.end();)
        {
            if(*it == get_current_player_substate()) {
                it = m_PlayerSubStates.erase(it);
            } else {
                ++it;
            }
        }
    }
private:
    
    
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
    
    FStateManager() {};

private:
    // typeof key, type of the element
    std::unordered_map<std::string, PlayerState*> m_PlayerStates;
    std::vector<std::string> m_PlayerSubStates;

    PlayerState* m_CurrentPlayerState = nullptr;
};