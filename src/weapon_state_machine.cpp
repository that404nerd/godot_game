#include "weapon_state_machine.h"
#include "weapon_manager.h"

void WeaponStateMachine::_init_data(WeaponManager* weaponManager, CharacterComponent* characterComponent)
{
  m_WeaponManagerInst = weaponManager;
  m_CharacterComponent = characterComponent;

  m_States[static_cast<uint8_t>(WeaponStates::IDLE)] = std::make_unique<WeaponIdleState>(weaponManager);
  m_States[static_cast<uint8_t>(WeaponStates::EQUIP)] = std::make_unique<WeaponEquipState>(weaponManager);
  m_States[static_cast<uint8_t>(WeaponStates::SHOOT)] = std::make_unique<WeaponShootState>(weaponManager);
  m_States[static_cast<uint8_t>(WeaponStates::RELOAD)] = std::make_unique<WeaponReloadState>(weaponManager);
 
  m_InitialState = m_States.at(static_cast<uint8_t>(WeaponStates::EQUIP)).get();

  CharacterBody3D* characterBody = m_CharacterComponent->get_character_body();
  m_WeaponAnimGroups = characterBody->get_tree()->get_nodes_in_group("weapon_anims");
}

void WeaponStateMachine::_bind_methods()
{

}