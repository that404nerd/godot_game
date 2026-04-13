#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

#include "state.h"
#include "state_machine.h"
#include "player.h"
#include "globals.h"

class PlayerDashState : public State {
  GDCLASS(PlayerDashState, State);

public:
  PlayerDashState() {}; 

  virtual void _enter() override;
  virtual void _handle_input(const Ref<InputEvent>& event) override;
  virtual void _physics_update(double delta) override;

  virtual void _exit() override;

protected:
  static void _bind_methods();

private:
  Player* m_PlayerInst { nullptr };
  StateMachine* m_StateMachineInst { nullptr };

  Vector3 m_DashDir;
};
