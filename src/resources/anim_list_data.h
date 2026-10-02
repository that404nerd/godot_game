#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_player.hpp>

#include "../globals.h"

using namespace godot;

class AnimListData : public Resource
{
  GDCLASS(AnimListData, Resource);

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(StringName, playback_path, StringName());
  GD_DEFINE_PROPERTY(StringName, action_name, StringName());
  GD_DEFINE_PROPERTY(StringName, underlying_anim_name, StringName());
  GD_DEFINE_PROPERTY(bool, has_valid_anim, false);
};