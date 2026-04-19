#include "weapon_state_machine.h"
#include "weapon_states.h"

void WeaponStateMachine::_init_data()
{
  m_States[static_cast<uint8_t>(WeaponStates::IDLE)] = std::make_unique<WeaponIdleState>(this);
  m_States[static_cast<uint8_t>(WeaponStates::EQUIP)] = std::make_unique<WeaponEquipState>(this);
  m_States[static_cast<uint8_t>(WeaponStates::SHOOT)] = std::make_unique<WeaponShootState>(this);
  m_States[static_cast<uint8_t>(WeaponStates::RELOAD)] = std::make_unique<WeaponReloadState>(this);
  m_States[static_cast<uint8_t>(WeaponStates::UNEQUIP)] = std::make_unique<WeaponUnequipState>(this);
  
  m_InitialState = m_States.at(static_cast<uint8_t>(WeaponStates::EQUIP)).get();
  
  CharacterBody3D* characterBody = character_component->get_character_body();
  m_WeaponAnimGroups = characterBody->get_tree()->get_nodes_in_group("weapon_anims");
}

void WeaponStateMachine::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponStateMachine, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponStateMachine, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}