#include "ai_behaviour_props.h"

void AIBehaviourProps::_bind_methods()
{
  GD_BIND_PROPERTY(AIBehaviourProps, enemyDistToTriggerCombat, Variant::FLOAT);
  GD_BIND_PROPERTY(AIBehaviourProps, enemyDistToShoot, Variant::FLOAT);
  GD_BIND_PROPERTY(AIBehaviourProps, enemyDistBetweenPlayer, Variant::FLOAT);
  GD_BIND_PROPERTY(AIBehaviourProps, playerDistToTriggerChase, Variant::FLOAT);
  GD_BIND_PROPERTY(AIBehaviourProps, enemyCombatNewPosQueryTime, Variant::FLOAT);
}