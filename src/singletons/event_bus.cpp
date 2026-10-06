#include "event_bus.h"

EventBus* EventBus::s_EventBus = nullptr;

EventBus::EventBus()
{
  s_EventBus = this;
}

void EventBus::_bind_methods()
{
  // For Weapon States
  ADD_SIGNAL(MethodInfo("weapon_fired", PropertyInfo(Variant::OBJECT, "currentWeapon"), PropertyInfo(Variant::OBJECT, "owner")));
  ADD_SIGNAL(MethodInfo("weapon_reload_start", PropertyInfo(Variant::OBJECT, "skeleton"), PropertyInfo(Variant::OBJECT, "owner")));
  ADD_SIGNAL(MethodInfo("weapon_switched", PropertyInfo(Variant::OBJECT, "currentWeapon"), PropertyInfo(Variant::OBJECT, "owner")));

  // For Ammo stuff
  ADD_SIGNAL(MethodInfo("ammo_finished"));

  // For the PlayAnimComponent interface
  ADD_SIGNAL(MethodInfo("anim_started", PropertyInfo(Variant::STRING_NAME, "animName")));
  ADD_SIGNAL(MethodInfo("anim_finished", PropertyInfo(Variant::STRING_NAME, "animName")));
}

EventBus* EventBus::get_singleton()
{
  return s_EventBus;
}

EventBus::~EventBus()
{

}
