#include "play_anim_from_player.h"

void PlayAnimFromPlayer::_ready()
{
  PlayAnimComponent::_ready();

  anim_player->connect("animation_started", Callable(this, "_on_anim_started"));
  anim_player->connect("animation_finished", Callable(this, "_on_anim_finished"));
}

void PlayAnimFromPlayer::_on_anim_started(StringName animName)
{
  EventBus::get_singleton()->emit_signal("anim_started", animName); 
}

void PlayAnimFromPlayer::_on_anim_finished(StringName animName)
{
  EventBus::get_singleton()->emit_signal("anim_finished", animName); 
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