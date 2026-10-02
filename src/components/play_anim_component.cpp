#include "play_anim_component.h"

void PlayAnimComponent::_ready()
{
  m_AnimsMap[AnimTypes::WEAPON_EQUIP] = "equip_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_SHOOT] = "shoot_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_RELOAD_START] = "reload_start_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_RELOAD] = "reload_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_RELOAD_END] = "reload_end_anim"; 
  m_AnimsMap[AnimTypes::WEAPON_UNEQUIP] = "unequip_anim"; 
}

void PlayAnimComponent::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_anim_started", "animName"), &PlayAnimComponent::_on_anim_started);
  ClassDB::bind_method(D_METHOD("_on_anim_finished", "animName"), &PlayAnimComponent::_on_anim_finished);
}