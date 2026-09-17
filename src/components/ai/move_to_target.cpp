#include "move_to_target.h"

void BTMoveToTarget::_setup()
{
  m_AICharacterComp = Object::cast_to<AICharacterComponent>(get_agent());
}

void BTMoveToTarget::_enter()
{
  m_AIManagerInst = m_AICharacterComp->get_ai_manager();
}

void BTMoveToTarget::_bind_methods()
{

}


BT::Status BTMoveToTarget::_tick(double delta)
{
  return m_AIManagerInst->_chase(delta);
}