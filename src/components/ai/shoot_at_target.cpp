#include "shoot_at_target.h"

void BTShootAtTarget::_setup()
{
  m_AICharacterComp = Object::cast_to<AICharacterComponent>(get_agent());
}

void BTShootAtTarget::_enter()
{
  m_AIManagerInst = m_AICharacterComp->get_ai_manager();
}

void BTShootAtTarget::_bind_methods()
{

}

BT::Status BTShootAtTarget::_tick(double delta)
{
  return m_AIManagerInst->_shoot(delta);
}