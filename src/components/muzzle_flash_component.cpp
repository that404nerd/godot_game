#include "muzzle_flash_component.h"

void MuzzleFlashComponent::_ready()
{
  set_process(false);
  set_physics_process(false);
}

void MuzzleFlashComponent::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(MuzzleFlashComponent, omni_light_inst, Variant::OBJECT, godot::PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(MuzzleFlashComponent, gpu_particles_inst, Variant::OBJECT, godot::PROPERTY_HINT_NODE_TYPE);
}
