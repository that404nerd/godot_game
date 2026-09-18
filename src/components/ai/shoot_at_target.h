#pragma once

#include <godot_cpp/godot.hpp>

#include "../../managers/ai_manager.h"

#include "ai_character_component.h"
#include "bt/tasks/bt_action.h"
#include "util/limbo_task_db.h"

using namespace godot;

class BTShootAtTarget : public BTAction
{
  GDCLASS(BTShootAtTarget, BTAction);
  TASK_CATEGORY(CustomActions)
public:
  void _setup() override;
  void _enter() override;

  BT::Status _tick(double delta) override;

protected:
  static void _bind_methods();

private:
  AICharacterComponent* m_AICharacterComp { nullptr };
  AIManager* m_AIManagerInst { nullptr };
};