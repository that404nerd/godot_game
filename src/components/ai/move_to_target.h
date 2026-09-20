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

  enum MoveType
  {
    WALK, RUN
  };

  BT::Status _tick(double delta) override;
protected:
  static void _bind_methods();

private:
  AICharacterComponent* m_AICharacterComp { nullptr };
  AIManager* m_AIManagerInst { nullptr };

  GD_DEFINE_PROPERTY(MoveType, move_type, MoveType::WALK);
  GD_DEFINE_PROPERTY(bool, should_rotate_towards_player, false);
  GD_DEFINE_PROPERTY(bool, should_move_to_player, false);
};

VARIANT_ENUM_CAST(BTMoveToTarget::MoveType);