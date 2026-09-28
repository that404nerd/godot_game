#pragma once

#include <godot_cpp/godot.hpp>

#include "../globals.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

typedef TypedDictionary<StringName, StringName> AnimList;

// enum class AnimList 
// {
//   IDLE, WALK, SPRINT, CROUCH,
//   WEAPON_IDLE, WEAPON_EQUIP, WEAPON_SHOOT, WEAPON_RELOAD, WEAPON_UNEQUIP
// };

class PlayAnimComponent : public Node {
  GDCLASS(PlayAnimComponent, Node);
public:
  virtual void execute_anim(const StringName& anim_name) = 0;
protected:
  static void _bind_methods();
private:
  GD_DEFINE_PROPRERTY(AnimList, anim_list, AnimList());
};