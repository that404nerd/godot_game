#include "weapon_state_machine.h"
#include "weapon_manager.h"

WeaponStateMachine::WeaponStateMachine(WeaponManager* weaponManager)
  : m_WeaponManagerInst(weaponManager)
{
  m_States[static_cast<uint8_t>(WeaponStates::IDLE)] = std::make_unique<WeaponIdleState>(weaponManager);
  m_States[static_cast<uint8_t>(WeaponStates::EQUIP)] = std::make_unique<WeaponEquipState>(weaponManager);
  m_States[static_cast<uint8_t>(WeaponStates::SHOOT)] = std::make_unique<WeaponShootState>(weaponManager);
  
   
  m_States[static_cast<uint8_t>(WeaponStates::RELOAD)] = std::make_unique<WeaponReloadState>(weaponManager);
 

  m_InitialState = m_States.at(static_cast<uint8_t>(WeaponStates::EQUIP)).get();
}