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

public:
  void set_animation_player(AnimationPlayer* anim_player) { m_AnimPlayer = anim_player; }
  AnimationPlayer* get_anim_player() { return m_AnimPlayer; }

  void do_something();

protected:
  static void _bind_methods();
  GD_DEFINE_COND_FUNCS()

private:
  AnimationPlayer* m_AnimPlayer { nullptr };
  PackedStringArray m_AnimNames {};

  GD_DEFINE_PROPERTY(int, selected_anim, 0);
};