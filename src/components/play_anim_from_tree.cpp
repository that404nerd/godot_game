#include "play_anim_from_tree.h"

void PlayAnimFromTree::_ready()
{
  PlayAnimComponent::_ready();

  if(!playback_state_path.is_empty())
    m_AnimTreePlayback = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get(playback_state_path));
  else
    print_error("Playback state path is empty!");
}

void PlayAnimFromTree::execute_anim(AnimTypes anim_type)
{
  StringName anim_name = get_anim(anim_type);
  m_AnimTreePlayback->travel(anim_name);
}

void PlayAnimFromTree::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(PlayAnimFromTree, AnimationTree, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_PROPERTY(PlayAnimFromTree, playback_state_path, Variant::STRING_NAME);
}