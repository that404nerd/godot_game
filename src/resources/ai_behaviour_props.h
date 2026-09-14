#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/resource.hpp>

#include "../globals.h"

using namespace godot;

class AIBehaviourProps : public Resource
{
  GDCLASS(AIBehaviourProps, Resource)
public:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(float, enemyDistToTriggerCombat, 10.0f);
  GD_DEFINE_PROPERTY(float, enemyDistToShoot, 5.0f);
  GD_DEFINE_PROPERTY(float, enemyChaseDistFromPlayer, 5.0f);
  GD_DEFINE_PROPERTY(float, playerDistToTriggerChase, 15.0f);
  GD_DEFINE_PROPERTY(float, enemyCombatNewPosQueryTime, 1.0f);
};