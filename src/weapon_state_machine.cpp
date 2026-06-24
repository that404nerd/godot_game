#include "weapon_state_machine.h"

void WeaponStateMachine::_init_data()
{
  m_WeaponStateData = { weapon_manager, this };

  m_States[static_cast<int8_t>(WeaponStates::IDLE)] = std::make_unique<WeaponIdleState>(m_WeaponStateData);
  m_States[static_cast<int8_t>(WeaponStates::EQUIP)] = std::make_unique<WeaponEquipState>(m_WeaponStateData);
  m_States[static_cast<int8_t>(WeaponStates::SHOOT)] = std::make_unique<WeaponShootState>(m_WeaponStateData);
  m_States[static_cast<int8_t>(WeaponStates::RELOAD)] = std::make_unique<WeaponReloadState>(m_WeaponStateData);
  m_States[static_cast<int8_t>(WeaponStates::UNEQUIP)] = std::make_unique<WeaponUnequipState>(m_WeaponStateData);
  m_States[static_cast<int8_t>(WeaponStates::WEAPON_SWITCH)] = std::make_unique<WeaponSwitchState>(m_WeaponStateData);
  
  m_InitialState = m_States.at(static_cast<int8_t>(WeaponStates::EQUIP)).get();
}

void WeaponStateMachine::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponStateMachine, weapon_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponStateMachine, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_PROPERTY(WeaponStateMachine, shoot_time_before_idle, Variant::FLOAT);

  ClassDB::bind_method(D_METHOD("_on_animation_finished", "anim_name"), &WeaponStateMachine::_on_animation_finished);
  ClassDB::bind_method(D_METHOD("get_current_state_name"), &WeaponStateMachine::get_current_state_name);
}

void WeaponStateMachine::_handle_state_machine_input(const Ref<InputEvent>& event)
{
  m_CurrentWeapon = weapon_manager->get_current_weapon();

  for(int i = 0; i < weapon_component->get_weapon_resource_list().size(); i++)
  {
    String inputAction = "weapon_" + String::num(i + 1, 0); // INFO: Need to match the set input action in the editor
    if(Input::get_singleton()->is_action_just_pressed(inputAction))
    {
      weapon_manager->get_weapon_state_ctx().IsReloading = false;
      weapon_manager->_switch_weapon_data(i);
      _change_state(static_cast<int8_t>(WeaponStates::UNEQUIP));
    }
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
    _change_state(static_cast<uint8_t>(WeaponStates::WEAPON_SWITCH));
  }
}
