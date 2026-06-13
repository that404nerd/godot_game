#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>

#include "muzzle_flash_component.h"

using namespace godot;

class WeaponWrapper : public Node {
  GDCLASS(WeaponWrapper, Node);

public:
  void _ready() override
  {

  }

  void _physics_process(double delta) override
  {
  }

protected:
  static void _bind_methods()
  {
    GD_BIND_CUSTOM_PROPERTY(WeaponWrapper, muzzle_flash_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponWrapper, armature_skeleton, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponWrapper, weapon_anim_player, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  }


private:
  GD_DEFINE_PROPERTY(MuzzleFlashComponent*, muzzle_flash_component, nullptr);
  GD_DEFINE_PROPERTY(Skeleton3D*, armature_skeleton, nullptr);
  GD_DEFINE_PROPERTY(AnimationPlayer*, weapon_anim_player, nullptr);
};
