#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_animation.hpp>
#include <godot_cpp/classes/animation_node_blend_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>

#include "../globals.h"
#include "play_anim_component.h"

using namespace godot;

typedef TypedDictionary<StringName, Ref<AnimListData>> AnimTreeList;

class PlayAnimFromTree : public PlayAnimComponent
{
  GDCLASS(PlayAnimFromTree, PlayAnimComponent);
public:
  void _ready() override;

  void execute_anim(AnimTypes animType) override;
  void _on_anim_started(StringName animName) override;
  void _on_anim_finished(StringName animName) override;

  StringName get_anim_name(AnimTypes animType) override;
  bool has_valid_anim(AnimTypes animType) override;

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(AnimationTree*, anim_tree, nullptr);
  GD_DEFINE_PROPERTY(bool, is_init, false);
  GD_DEFINE_PROPERTY(AnimTreeList, anim_list, AnimTreeList());


  AnimationNodeStateMachinePlayback* m_AnimTreePlayback { nullptr };
};