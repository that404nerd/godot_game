#include "play_anim_from_player.h"

void PlayAnimFromPlayer::_ready()
{
  PlayAnimComponent::_ready();

  if(Engine::get_singleton()->is_editor_hint() && !is_init)
  {
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_EQUIP).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_SHOOT).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_RELOAD_START).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_RELOAD).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_RELOAD_END).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_UNEQUIP).c_str(), StringName());
  }
  is_init = true;

  anim_player->connect("animation_started", Callable(this, "_on_anim_started"));
  anim_player->connect("animation_finished", Callable(this, "_on_anim_finished"));
}

void PlayAnimFromPlayer::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(PlayAnimFromPlayer, AnimationPlayer, anim_player, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_PROPERTY(PlayAnimFromPlayer, is_init, Variant::BOOL);
  GD_BIND_PROPERTY(PlayAnimFromPlayer, anim_list, Variant::DICTIONARY);
}

void PlayAnimFromPlayer::_on_anim_started(StringName animName)
{
  EventBus::get_singleton()->emit_signal("anim_started", animName); 
}

void PlayAnimFromPlayer::_on_anim_finished(StringName animName)
{
  EventBus::get_singleton()->emit_signal("anim_finished", animName); 
}

StringName PlayAnimFromPlayer::get_anim_name(AnimTypes animType)
{
  std::string animKey = m_AnimsMap.at(animType);
  if(anim_list.has(animKey.c_str()))
  {
    return anim_list.get(animKey.c_str(), StringName()); 
  }

  print_error("Anim doesn't exist!");
  return StringName();
}

void PlayAnimFromPlayer::execute_anim(AnimTypes anim_type)
{
  StringName anim_name = get_anim_name(anim_type);
  anim_player->play(anim_name);
}