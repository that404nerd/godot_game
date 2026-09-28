#include "play_anim_from_player.h"

void PlayAnimFromPlayer::execute_anim(const StringName& anim_name)
{
  anim_player->play(anim_name);
}

void PlayAnimFromPlayer::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(PlayAnimFromPlayer, AnimationPlayer, anim_player, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}