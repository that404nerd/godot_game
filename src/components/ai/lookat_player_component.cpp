#include "lookat_player_component.h"

#include "../../player.h"

void LookAtPlayerComponent::_init(Player* playerInst)
{
  m_PlayerInst = playerInst;
}

void LookAtPlayerComponent::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(LookAtPlayerComponent, LookAtModifier3D, lookAtModiferInst, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void LookAtPlayerComponent::_update(double delta) 
{
  lookAtModiferInst->set_target_node(m_PlayerInst->get_path());
}