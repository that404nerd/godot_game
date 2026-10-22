#include "damage_system.h"

void DamageSystem::_ready()
{
  if(hitboxType == HitBoxType::BoneHitbox)
  {
    for(int i = 0; i < ragdoll_simulator->get_child_count(); i++)
    {
      PhysicalBoneHitbox* boneHitbox = Object::cast_to<PhysicalBoneHitbox>(ragdoll_simulator->get_children()[i]);
      HitBoxComponent* hitboxInst = boneHitbox->get_boneHitBox();
      hitboxInst->connect("hitbox_collided", Callable(this, "_on_hit"));
    }
  } else {
    genericHitboxComp->connect("hitbox_collided", Callable(this, "_on_hit"));
  }
}

void DamageSystem::_on_hit(float totalDamage, Node* hitSource, Node* hitTarget)
{
  healthComp->_take_damage(totalDamage, hitTarget);
}

void DamageSystem::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_hit", "totalDamage", "hitSource", "hitTarget"), &DamageSystem::_on_hit);

  BIND_ENUM_CONSTANT(BoneHitbox);
  BIND_ENUM_CONSTANT(GenericHitbox);
  GD_BIND_ENUM(DamageSystem, hitboxType, "BoneHitbox,GenericHitbox");

  GD_BIND_CUSTOM_PROPERTY(DamageSystem, PhysicalBoneSimulator3D, ragdoll_simulator, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(DamageSystem, HitBoxComponent, genericHitboxComp, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(DamageSystem, HealthComponent, healthComp, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}