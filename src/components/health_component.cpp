#include "health_component.h"

void HealthComponent::_ready()
{
}

void HealthComponent::_take_damage(int damage)
{
  if(max_health == 0)
    return;

  max_health -= damage;
  print_line(max_health);
}

void HealthComponent::_bind_methods()
{
  ADD_SIGNAL(MethodInfo("health_changed", PropertyInfo(Variant::INT, "currentHealth"), PropertyInfo(Variant::INT, "maxHealth")));
  ADD_SIGNAL(MethodInfo("isDead"));

  GD_BIND_PROPERTY(HealthComponent, max_health, Variant::INT);
}
