#include "ai_play_anim_from_tree.h"

void AIPlayAnimFromTree::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(AIPlayAnimFromTree, AnimationTree, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

String AIPlayAnimFromTree::_generate_name()
{
  return "Play Animation From Tree!";
}

void AIPlayAnimFromTree::_setup()
{
  print_line(anim_tree);  
}

void AIPlayAnimFromTree::_enter()
{

}

BT::Status AIPlayAnimFromTree::_tick(double delta)
{
  return SUCCESS;
}

