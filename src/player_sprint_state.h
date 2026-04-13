#pragma once

#include "state.h"
#include "state_machine.h"
#include "player.h"

#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

#include "globals.h"

#include "dd3d_cpp_api.hpp"

class PlayerSprintState : public State {
  GDCLASS(PlayerSprintState, State);

public:
  PlayerSprintState() {}; 

  virtual void _enter() override;
  virtual void _handle_input(const Ref<InputEvent>& event) override;
  virtual void _physics_update(double delta) override;
  virtual void _exit() override; 

  void _headbob_effect(double delta);

protected:
  static void _bind_methods();

private:
  Player* m_PlayerInst;
  StateMachine* m_StateMachineInst;

};
