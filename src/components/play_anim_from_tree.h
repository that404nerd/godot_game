#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>

#include "../globals.h"
#include "play_anim_component.h"

using namespace godot;

class PlayAnimFromTree : public PlayAnimComponent
{
  GDCLASS(PlayAnimFromTree, PlayAnimComponent);
public:
  void _ready() override;

  void execute_anim(AnimTypes anim_type) override;

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(AnimationTree*, anim_tree, nullptr);
  GD_DEFINE_PROPERTY(StringName, playback_state_path, StringName());

  AnimationNodeStateMachinePlayback* m_AnimTreePlayback { nullptr };
};