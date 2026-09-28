#pragma once

#include <unordered_map>

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "../globals.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

enum class AnimTypes
{
  IDLE, WALK, SPRINT, CROUCH,
  WEAPON_EQUIP, WEAPON_SHOOT, WEAPON_RELOAD_START, WEAPON_RELOAD, WEAPON_RELOAD_END, WEAPON_UNEQUIP
};

typedef TypedDictionary<StringName, StringName> AnimList;

class PlayAnimComponent : public Node {
  GDCLASS(PlayAnimComponent, Node);
public:

  void _ready() override;
  virtual void execute_anim(AnimTypes anim_type) = 0;

  StringName get_anim(AnimTypes animType);

protected:
  static void _bind_methods();


protected:
  GD_DEFINE_PROPERTY(AnimList, anim_list, AnimList());
  std::unordered_map<AnimTypes, std::string> m_AnimsMap;
};