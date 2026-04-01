#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>

#include "weapon_manager.h"

using namespace godot;

class WeaponState : public Node
{
  GDCLASS(WeaponState, Node)
public:
  void _weapon_idle(double delta);
  void _weapon_shoot(double delta);
  void _weapon_reload(double delta);

  void _ready() override;
  void _physics_process(double delta) override;

protected:
  static void _bind_methods();

private:
  WeaponManager* m_WeaponManager; 
  PlayerStateMachine* m_StateMachineInst;
};
