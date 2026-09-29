#include "play_anim_component.h"

void PlayAnimComponent::_ready()
{
  m_AnimsMap[AnimTypes::WEAPON_EQUIP] = "equip_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_SHOOT] = "shoot_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_RELOAD_START] = "reload_start_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_RELOAD] = "reload_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_RELOAD_END] = "reload_end_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_UNEQUIP] = "unequip_anim"; 

  is_init = true;
  // A small little trick to not set the dictionary values to empty string everytime the editor restarts (is_init is serialized)
  if(Engine::get_singleton()->is_editor_hint() && !is_init)
  {
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_EQUIP).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_SHOOT).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_RELOAD_START).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_RELOAD).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_RELOAD_END).c_str(), StringName());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_UNEQUIP).c_str(), StringName());
  }
}

void PlayAnimComponent::_bind_methods()
{
  GD_BIND_PROPERTY(PlayAnimComponent, is_init, Variant::BOOL);
  GD_BIND_PROPERTY(PlayAnimComponent, anim_list, Variant::DICTIONARY);

  ClassDB::bind_method(D_METHOD("_on_anim_started", "animName"), &PlayAnimComponent::_on_anim_started);
  ClassDB::bind_method(D_METHOD("_on_anim_finished", "animName"), &PlayAnimComponent::_on_anim_finished);
}

StringName PlayAnimComponent::get_anim(AnimTypes animType)
{
  std::string animKey = m_AnimsMap.at(animType);
  if(anim_list.has(animKey.c_str()))
  {
    return anim_list.get(animKey.c_str(), StringName()); 
  }

  print_error("Anim doesn't exist!");
  return StringName();
}