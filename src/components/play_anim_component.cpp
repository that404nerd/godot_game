#include "play_anim_component.h"

void PlayAnimComponent::_ready()
{
  m_AnimsMap[AnimTypes::WEAPON_EQUIP] = "equip_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_SHOOT] = "shoot_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_RELOAD_START] = "reload_start_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_RELOAD] = "reload_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_RELOAD_END] = "reload_end_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_UNEQUIP] = "unequip_anim"; 

  if(Engine::get_singleton()->is_editor_hint())
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
  GD_BIND_PROPERTY(PlayAnimComponent, anim_list, Variant::DICTIONARY);
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