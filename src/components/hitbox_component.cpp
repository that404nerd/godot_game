#include "hitbox_component.h"

void HitBoxComponent::_ready()
{
  connect("hitbox_collided", Callable(this, "_on_hit"));
}

void HitBoxComponent::_on_hit(float totalDamage, Node* hitSource, Node* hitTarget)
{
  print_line(hitSource->get_name(), " ", hitTarget->get_name());
}

void HitBoxComponent::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_hit", "totalDamage", "hitSource", "hitTarget"), &HitBoxComponent::_on_hit);

  GD_BIND_PROPERTY(HitBoxComponent, damage_multiplier, Variant::FLOAT);
  GD_BIND_CUSTOM_PROPERTY(HitBoxComponent, CollisionShape3D, hitboxCollider, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ADD_SIGNAL(MethodInfo("hitbox_collided", PropertyInfo(Variant::INT, "totalDamageTaken"), PropertyInfo(Variant::OBJECT, "hitSource"), PropertyInfo(Variant::OBJECT, "hitTarget")));
}