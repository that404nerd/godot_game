#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/marker3d.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include "muzzle_flash_component.h"
#include "play_anim_component.h"

using namespace godot;

class WeaponWrapper : public Node3D {
  GDCLASS(WeaponWrapper, Node3D);

protected:
  static void _bind_methods()
  {
    GD_BIND_PROPERTY(WeaponWrapper, mesh_instances, Variant::ARRAY);
    GD_BIND_PROPERTY(WeaponWrapper, anim_component_required, Variant::BOOL);

    ADD_GROUP("Nodes", "");
    GD_BIND_CUSTOM_PROPERTY(WeaponWrapper, Node3D, armature_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponWrapper, Marker3D, muzzle_point_marker, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponWrapper, MuzzleFlashComponent, muzzle_flash_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponWrapper, Skeleton3D, armature_skeleton, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponWrapper, AnimationPlayer, weapon_anim_player, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponWrapper, PlayAnimComponent, play_anim_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  }


private:
  GD_DEFINE_PROPERTY(Node3D*, armature_node, nullptr);
  GD_DEFINE_PROPERTY(Marker3D*, muzzle_point_marker, nullptr);
  GD_DEFINE_PROPERTY(MuzzleFlashComponent*, muzzle_flash_component, nullptr);
  GD_DEFINE_PROPERTY(Skeleton3D*, armature_skeleton, nullptr);
  GD_DEFINE_PROPERTY(AnimationPlayer*, weapon_anim_player, nullptr);
  GD_DEFINE_PROPERTY(PlayAnimComponent*, play_anim_component, nullptr);
  GD_DEFINE_PROPERTY(Array, mesh_instances, Array());

  // TODO: Use this to either show or hide the play_anim_component option
  GD_DEFINE_PROPERTY(bool, anim_component_required, true);
};
