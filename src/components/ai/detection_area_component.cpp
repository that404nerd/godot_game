#include "detection_area_component.h"

void DetectionAreaComponent::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(DetectionAreaComponent, CollisionShape3D, detection_collision_shape, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}