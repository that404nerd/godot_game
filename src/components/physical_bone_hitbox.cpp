#include "physical_bone_hitbox.h"

void PhysicalBoneHitbox::_ready()
{
  // set the layer to (1 << 3) -> layer 4 (enemy_physical_bones)
  set_collision_layer(1 << 3);

  // set the mash to 11 -> layer 1, 2, 4 (environment, player and enemy_physical_bones)
  set_collision_mask((1 << 0) | (1 << 1) | (1 << 3));

  for(int i = 0; i < get_child_count(); i++)
  {
    Node* childNode = Object::cast_to<Node>(get_children()[i]);
    if(childNode->get_class() == HitBoxComponent::get_class_static())
    {
      boneHitBox = Object::cast_to<HitBoxComponent>(childNode);
    }
  }
}

void PhysicalBoneHitbox::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(PhysicalBoneHitbox, HitBoxComponent, boneHitBox, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}