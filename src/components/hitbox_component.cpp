#include "hitbox_component.h"

void HitBoxComponent::_ready()
{
}


void HitBoxComponent::_bind_methods()
{
  GD_BIND_PROPERTY(HitBoxComponent, damage_multiplier, Variant::FLOAT);
  GD_BIND_CUSTOM_PROPERTY(HitBoxComponent, CollisionShape3D, hitboxCollider, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ADD_SIGNAL(MethodInfo("hitbox_collided", PropertyInfo(Variant::INT, "totalDamageTaken"), PropertyInfo(Variant::OBJECT, "hitSource"), PropertyInfo(Variant::OBJECT, "hitTarget")));
}