#include "health_component.h"

void HealthComponent::_ready()
{
  m_CurrentHealth = max_health;
}

void HealthComponent::_take_damage(float damage)
{
  if(m_CurrentHealth <= 0.0f)
    return;

  float actual_damage = Math::max(0.0f, damage);
  m_CurrentHealth = Math::max(0.0f, m_CurrentHealth - actual_damage);

  if(m_CurrentHealth <= 0.0f)
  {
    emit_signal("death");
  } else {
    emit_signal("health_changed", m_CurrentHealth, max_health);
  }
}

void HealthComponent::_bind_methods()
{
  GD_BIND_PROPERTY(HealthComponent, max_health, Variant::FLOAT);

  ADD_SIGNAL(MethodInfo("health_changed", PropertyInfo(Variant::FLOAT, "currentHealth"), PropertyInfo(Variant::FLOAT, "maxHealth")));
  ADD_SIGNAL(MethodInfo("death"));
}
