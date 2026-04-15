#include "global_state_handler.h"
#include "player.h"

GlobalStateHandler::GlobalStateHandler(Player* player)
{
  m_PlayerInst = player;
}

void GlobalStateHandler::_enter()
{
}

void GlobalStateHandler::_physics_update(double delta)
{
  if(!m_PlayerInst->get_global_state().CanDash)
  {
    m_PlayerInst->get_global_state().DashCooldown -= delta * 1.2f;

    if(m_PlayerInst->get_global_state().DashCooldown < 0.0f)
      return;
  }
}

GlobalStateHandler::~GlobalStateHandler()
{

}
