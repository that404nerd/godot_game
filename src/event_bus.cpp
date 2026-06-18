#include "event_bus.h"

EventBus* EventBus::s_EventBus = nullptr;

EventBus::EventBus()
{
  s_EventBus = this;
}

void EventBus::_bind_methods()
{
  ADD_SIGNAL(MethodInfo("weapon_fired", PropertyInfo(Variant::STRING, "nextWeaponName")));
  ADD_SIGNAL(MethodInfo("weapon_reload_start", PropertyInfo(Variant::STRING, "nextWeaponName")));
  ADD_SIGNAL(MethodInfo("weapon_reload_end", PropertyInfo(Variant::STRING, "nextWeaponName")));
}

EventBus* EventBus::get_singleton()
{
  return s_EventBus;
}

EventBus::~EventBus()
{

}
