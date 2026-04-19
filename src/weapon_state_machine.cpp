#include "weapon_state_machine.h"
#include "weapon_manager.h"

void WeaponStateMachine::_init_data(const WeaponStateMachineData& weaponStateMachineData)
{
  m_States[static_cast<uint8_t>(WeaponStates::IDLE)] = std::make_unique<WeaponIdleState>(this);
  m_States[static_cast<uint8_t>(WeaponStates::EQUIP)] = std::make_unique<WeaponEquipState>(this);
  m_States[static_cast<uint8_t>(WeaponStates::SHOOT)] = std::make_unique<WeaponShootState>(this);
  m_States[static_cast<uint8_t>(WeaponStates::RELOAD)] = std::make_unique<WeaponReloadState>(this);
  
  m_InitialState = m_States.at(static_cast<uint8_t>(WeaponStates::EQUIP)).get();
  
  m_CharacterComponent = weaponStateMachineData.characterComponent;
  
  CharacterBody3D* characterBody = m_CharacterComponent->get_character_body();
  m_WeaponAnimGroups = characterBody->get_tree()->get_nodes_in_group("weapon_anims");

  m_WeaponComponent = weaponStateMachineData.weaponComponent;
}

void WeaponStateMachine::_bind_methods()
{

}