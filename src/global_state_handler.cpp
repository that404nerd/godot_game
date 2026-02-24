#include "global_state_handler.h"

#include "player.h"

GlobalStateHandler::GlobalStateHandler()
{
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
}

void GlobalStateHandler::_ready()
{
}

void GlobalStateHandler::_bind_methods()
{

}

void GlobalStateHandler::_physics_process(double delta)
{
  if(m_PlayerInst->get_global_state().JumpBufferCooldown > 0.0f)
  {
    m_PlayerInst->get_global_state().JumpBufferCooldown -= delta;
  }

}

GlobalStateHandler::~GlobalStateHandler()
{

}
