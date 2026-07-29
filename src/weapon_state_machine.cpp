#include "weapon_state_machine.h"
#include "weapon_manager.h"

void WeaponStateMachine::_init_data()
{
  m_WeaponStateData = { weapon_manager, this };
  if(weapon_manager == nullptr || weapon_component == nullptr)
  {
    print_error("Weapon Manager is invalid!");
    return;
  }

  m_States[static_cast<int>(WeaponStates::IDLE)] = std::make_unique<WeaponIdleState>(m_WeaponStateData);
  m_States[static_cast<int>(WeaponStates::EQUIP)] = std::make_unique<WeaponEquipState>(m_WeaponStateData);
  m_States[static_cast<int>(WeaponStates::SHOOT)] = std::make_unique<WeaponShootState>(m_WeaponStateData);
  m_States[static_cast<int>(WeaponStates::RELOAD)] = std::make_unique<WeaponReloadState>(m_WeaponStateData);
  m_States[static_cast<int>(WeaponStates::UNEQUIP)] = std::make_unique<WeaponUnequipState>(m_WeaponStateData);
  m_States[static_cast<int>(WeaponStates::WEAPON_SWITCH)] = std::make_unique<WeaponSwitchState>(m_WeaponStateData);
  
  m_InitialState = m_States.at(static_cast<int>(WeaponStates::EQUIP)).get();

  m_CmdSystem = weapon_manager->get_input_command_system_instance();
  m_CmdSystem->set_weapon_list_size(weapon_component->get_weapon_resource_list().size());
}

void WeaponStateMachine::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponStateMachine, weapon_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponStateMachine, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ClassDB::bind_method(D_METHOD("_on_animation_finished", "anim_name"), &WeaponStateMachine::_on_animation_finished);
  ClassDB::bind_method(D_METHOD("get_current_state_name"), &WeaponStateMachine::get_current_state_name);
}

void WeaponStateMachine::_handle_state_machine_input(const Ref<InputEvent>& event)
{
  if(m_CmdSystem->wants_to_switch_weapon())
  {
    weapon_manager->get_weapon_state_ctx().IsReloading = false;
    weapon_manager->_switch_weapon_data(m_CmdSystem->get_weapon_idx());
    _change_state(static_cast<int>(WeaponStates::UNEQUIP));
  }
}

StringName WeaponStateMachine::get_current_state_name()
{
  auto none_name = magic_enum::enum_name(WeaponStates::NONE);
  StringName noneString = std::string(none_name).c_str();
  if(m_CurrentState == nullptr)
  {
    print_error("Current state is null!");
    return noneString;
  }

  auto state = magic_enum::enum_cast<WeaponStates>(m_CurrentState->get_current_state());
  auto stateName = magic_enum::enum_name(state.value());
  StringName finalStateName = std::string(stateName).c_str();

  return finalStateName;
}

void WeaponStateMachine::_on_animation_finished(const StringName& anim_name)
{
  if(anim_name == weapon_component->get_current_weapon_data()->get_weaponUnequipAnimName())
  {
    weapon_manager->_weapon_unequip_over();
    _change_state(static_cast<int>(WeaponStates::WEAPON_SWITCH));
  }
}
