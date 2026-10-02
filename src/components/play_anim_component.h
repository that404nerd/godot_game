#pragma once

#include <unordered_map>

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "../globals.h"
#include "../resources/anim_list_data.h"
#include "../singletons/event_bus.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

enum class AnimTypes
{
  IDLE, WALK, SPRINT, CROUCH,
  WEAPON_EQUIP, WEAPON_SHOOT, WEAPON_RELOAD_START, WEAPON_RELOAD, WEAPON_RELOAD_END, WEAPON_UNEQUIP
};


class PlayAnimComponent : public Node {
  GDCLASS(PlayAnimComponent, Node);
public:

  void _ready() override;
  virtual void execute_anim(AnimTypes anim_type) = 0;
  virtual void _on_anim_started(StringName animName) = 0;
  virtual void _on_anim_finished(StringName animName) = 0;

  virtual StringName get_anim_name(AnimTypes animType) = 0;
  
protected:
  static void _bind_methods();

protected:
  std::unordered_map<AnimTypes, std::string> m_AnimsMap;
};