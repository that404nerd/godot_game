#include "game_manager.h"
#include "player.h"
#include "state_machine.h"
#include "weapon_manager.h"

GameManager* GameManager::s_Singleton = nullptr;

GameManager::GameManager()
{
  s_Singleton = this;
}

void GameManager::_bind_methods()
{

}

void GameManager::set_player_inst(Player* player_inst)
{
  m_PlayerInst = player_inst;
}

Player* GameManager::get_player_inst()
{
  if(m_PlayerInst != nullptr)
    return m_PlayerInst;
  else
    print_error("Player Instance is null!");

  return nullptr;
}

StateMachine* GameManager::get_player_state_machine()
{
  if(m_PlayerInst != nullptr)
    return m_StateMachineInst;
  else
    print_error("Player State Machine Instance is null!");

  return nullptr;

}

void GameManager::set_state_machine_inst(StateMachine* stateMachine)
{
  m_StateMachineInst = stateMachine;
}

GameManager* GameManager::get_singleton() 
{ 
  return s_Singleton; 
}

GameManager::~GameManager()
{
  s_Singleton = nullptr; 
}
