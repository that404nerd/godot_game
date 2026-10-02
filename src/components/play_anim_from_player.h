#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/animation_player.hpp>

#include "../globals.h"
#include "./play_anim_component.h"

using namespace godot;

typedef TypedDictionary<StringName, StringName> AnimList;

class PlayAnimFromPlayer : public PlayAnimComponent {
  GDCLASS(PlayAnimFromPlayer, PlayAnimComponent);

public:
  void _ready() override;
  void execute_anim(AnimTypes anim_type) override;

  void _on_anim_started(StringName animName) override;
  void _on_anim_finished(StringName animName) override;
  StringName get_anim_name(AnimTypes animType) override;

  bool has_valid_anim(AnimTypes animType) override { return true; }

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(AnimationPlayer*, anim_player, nullptr);
  GD_DEFINE_PROPERTY(AnimList, anim_list, AnimList());
  GD_DEFINE_PROPERTY(bool, is_init, false);
};
