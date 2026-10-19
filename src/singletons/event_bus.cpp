#include "event_bus.h"

EventBus* EventBus::s_EventBus = nullptr;

EventBus::EventBus()
{
  s_EventBus = this;
}

void EventBus::_bind_methods()
{
  // For Weapon States

  // BUG: The owner property doesn't really do anything even when I emit the signal, the enemy's shot also causes recoil to the player's camera.
  ADD_SIGNAL(MethodInfo("weapon_fired", PropertyInfo(Variant::OBJECT, "currentWeapon"), PropertyInfo(Variant::OBJECT, "owner")));
  ADD_SIGNAL(MethodInfo("weapon_reload_start", PropertyInfo(Variant::OBJECT, "skeleton"), PropertyInfo(Variant::OBJECT, "owner")));
  ADD_SIGNAL(MethodInfo("weapon_switched", PropertyInfo(Variant::OBJECT, "currentWeapon"), PropertyInfo(Variant::OBJECT, "owner")));
}

EventBus* EventBus::get_singleton()
{
  return s_EventBus;
}

EventBus::~EventBus()
{

}