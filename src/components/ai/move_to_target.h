#pragma once

#include <godot_cpp/godot.hpp>

#include <bt/tasks/bt_action.h>

#include "../../managers/ai_manager.h"
#include "ai_character_component.h"

using namespace godot;

class BTMoveToTarget : public BTAction
{
  GDCLASS(BTMoveToTarget, BTAction);
  TASK_CATEGORY(CustomActions);

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