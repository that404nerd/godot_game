#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/animation_tree.hpp>

#include "bt/tasks/bt_action.h"
#include "bt/tasks/bt_task.h"

#include "../../globals.h"

class AIPlayAnimFromTree : public BTAction
{
  GDCLASS(AIPlayAnimFromTree, BTAction);
  TASK_CATEGORY(Scene);
public:
  virtual String _generate_name() override;
	virtual void _setup() override;
	virtual void _enter() override;
  Status _tick(double delta) override;

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(AnimationTree*, anim_tree, nullptr);
};