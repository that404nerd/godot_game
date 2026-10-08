#include "health_component.h"

void HealthComponent::_ready()
{
  m_CurrentHealth = max_health;
}

void HealthComponent::_take_damage(int damage, Node* hitBody)
{
  if(m_CurrentHealth == 0)
    return;

  m_CurrentHealth -= damage;
  
  print_line(hitBody->get_name(), " current health is: ", m_CurrentHealth);

  if(m_CurrentHealth <= 0.0f)
  {
    EventBus::get_singleton()->emit_signal("death");
  } else {
    EventBus::get_singleton()->emit_signal("health_changed", m_CurrentHealth, max_health);
  }
}

void HealthComponent::_bind_methods()
{
  GD_BIND_PROPERTY(HealthComponent, max_health, Variant::INT);
}
