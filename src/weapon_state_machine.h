#pragma once

#include "state_machine.h"
#include "weapon_states.h"

#include <memory>

class WeaponManager;
class WeaponIdleState;
class WeaponEquipState;
class WeaponShootState;
  
   
class WeaponReloadState;
 

enum class WeaponStates : uint8_t {
  IDLE, EQUIP, SHOOT, RELOAD, UNEQUIP, WEAPON_SWITCH
};

class WeaponStateMachine : public StateMachine {
public:
  WeaponStateMachine(WeaponManager* weaponManager);

  WeaponManager* get_weapon_manager_inst() const { return m_WeaponManagerInst; }

private:
  WeaponManager* m_WeaponManagerInst { nullptr };
};