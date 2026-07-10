#pragma once

#include "magic_enum/magic_enum.hpp"

#include "globals.h"

#include "movement_state_machine.h"
#include "movement_manager.h"
#include "state_machine.h"

#include "components/character_component.h"

struct MovementStateData
{
  MovementManager* MovementManagerInst;
  MovementStateMachine* MovementStateMachineInst;
};

enum class MovementStates {
  NONE = -1, IDLE, SPRINT, JUMP, FALL, CROUCH, SLIDE, DASH
};

class MovementStateMachine : public StateMachine 
{
  GDCLASS(MovementStateMachine, StateMachine);

public:
  void _init_data() override;

  StringName get_current_state_name()
  {
    auto none_name = magic_enum::enum_name(MovementStates::NONE);
    StringName noneString = std::string(none_name).c_str();
    if(m_CurrentState == nullptr)
    {
      print_error("Current state is null!");
      return noneString;
    }

    auto state = magic_enum::enum_cast<MovementStates>(m_CurrentState->get_current_state());
    auto stateName = magic_enum::enum_name(state.value());
    StringName finalStateName = std::string(stateName).c_str();

    return finalStateName;
  }

  StringName get_prev_state_name()
  {
    auto none_name = magic_enum::enum_name(MovementStates::NONE);
    StringName noneString = std::string(none_name).c_str();

    if(m_PrevState == nullptr)
    {
      print_error("Prev state doesn't exist!");
      return noneString;
    }

    auto state = magic_enum::enum_cast<MovementStates>(m_PrevState->get_current_state());
    auto stateName = magic_enum::enum_name(state.value());
    StringName finalStateName = std::string(stateName).c_str();


    return finalStateName;
  }

protected:
  static void _bind_methods();

private:

  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(MovementManager*, movement_manager, nullptr);

  MovementStateData m_MovementStateData;
};
