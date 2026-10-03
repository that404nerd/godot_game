#pragma once

#include "magic_enum/magic_enum.hpp"

#include "../globals.h"

#include "movement_state_machine.h"
#include "../managers/movement_manager.h"
#include "./state_machine.h"

#include "../components/character_component.h"

struct MovementStateData
{
  MovementManager* MovementManagerInst;
  MovementStateMachine* MovementStateMachineInst;
};

enum class MovementStates {
  NONE = -1, IDLE, WALK, SPRINT, JUMP, FALL, CROUCH, SLIDE, DASH
};

class MovementStateMachine : public StateMachine 
{
  GDCLASS(MovementStateMachine, StateMachine);

public:
  void _init_data() override;

  StringName get_current_state_name();
  StringName get_prev_state_name();

protected:
  static void _bind_methods();

private:

  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(MovementManager*, movement_manager, nullptr);

  MovementStateData m_MovementStateData;
};
