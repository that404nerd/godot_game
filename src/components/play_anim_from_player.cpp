#include "play_anim_from_player.h"

void PlayAnimFromPlayer::_ready()
{
  PlayAnimComponent::_ready();
}

void PlayAnimFromPlayer::execute_anim(AnimTypes anim_type)
{
  StringName anim_name = get_anim(anim_type);
  anim_player->play(anim_name);
}

void PlayAnimFromPlayer::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(PlayAnimFromPlayer, AnimationPlayer, anim_player, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}