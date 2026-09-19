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
  BIND_ENUM_CONSTANT(WALK);
  BIND_ENUM_CONSTANT(RUN);
  GD_BIND_ENUM(BTMoveToTarget, move_type, "Walk,Run");
  GD_BIND_PROPERTY(BTMoveToTarget, should_rotate_towards_player, Variant::BOOL);
}

BT::Status BTMoveToTarget::_tick(double delta)
{
  if(move_type == MoveType::RUN)
    return m_AIManagerInst->_chase(delta, should_rotate_towards_player);
  else
    return m_AIManagerInst->_patrol(delta, should_rotate_towards_player);

  return BT::FAILURE;
}