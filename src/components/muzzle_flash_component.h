#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/omni_light3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>

#include "../globals.h"
#include "../resources/weapon.h"

using namespace godot;

class MuzzleFlashComponent : public Node3D {
  GDCLASS(MuzzleFlashComponent, Node3D);
public:
  void _ready() override;

  void _set_particles_status(bool status) { gpu_particles_inst->set_emitting(status); }
  void _enable_light_status(bool status) { omni_light_inst->set_visible(status); }

  bool get_light_status() { return omni_light_inst->is_visible(); }
  float get_particle_lifetime() { return gpu_particles_inst->get_lifetime(); }

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(OmniLight3D*, omni_light_inst, nullptr);
  GD_DEFINE_PROPERTY(GPUParticles3D*, gpu_particles_inst, nullptr);
};