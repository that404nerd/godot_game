#include "weapon_states.h"
#include "weapon_state_machine.h"
#include "weapon_manager.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Idle State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponIdleState::WeaponIdleState(const WeaponStateData& weaponStateData)
  : State(static_cast<int8_t>(WeaponStates::IDLE)), m_WeaponManager(weaponStateData.weaponManager),
    m_WeaponStateMachine(weaponStateData.weaponStateMachine),
    m_WeaponStateContext(m_WeaponManager->get_weapon_state_ctx())
{
}

void WeaponIdleState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WeaponStateContext.IsKeyPressed = true;
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::SHOOT));
  }

  if(Input::get_singleton()->is_action_just_pressed("reload_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::RELOAD));
  }
  
}

void WeaponIdleState::_enter()
{
  if(m_WeaponManager == nullptr || m_WeaponStateMachine == nullptr)
  {
    print_error("Weapon Idle state data is null!");
    return;
  }
}

void WeaponIdleState::_update(double delta)
{
}

void WeaponIdleState::_exit()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Equip State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponEquipState::WeaponEquipState(const WeaponStateData& weaponStateData)
  : State(static_cast<int8_t>(WeaponStates::EQUIP)), m_WeaponManager(weaponStateData.weaponManager),
    m_WeaponStateMachine(weaponStateData.weaponStateMachine)
{
}

void WeaponEquipState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::SHOOT));
  }
}

void WeaponEquipState::_enter()
{
  if(m_WeaponManager == nullptr || m_WeaponStateMachine == nullptr)
  {
    print_error("Weapon Equip state data is null!");
    return;
  }

  m_WeaponManager->_equip_weapon();
}

void WeaponEquipState::_update(double delta)
{
  m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::IDLE));
}


void WeaponEquipState::_exit()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Shoot State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponShootState::WeaponShootState(const WeaponStateData& weaponStateData)
  : State(static_cast<int8_t>(WeaponStates::SHOOT)), m_WeaponManager(weaponStateData.weaponManager),
    m_WeaponStateMachine(weaponStateData.weaponStateMachine),
    m_WeaponStateContext(m_WeaponManager->get_weapon_state_ctx())
{
}

void WeaponShootState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("reload_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::RELOAD));
  }

}

void WeaponShootState::_enter()
{
  m_CurrentWeapon = m_WeaponManager->get_current_weapon();
}

void WeaponShootState::_update(double delta)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon") && m_WeaponManager->get_current_weapon_ammo() == 0 && m_CurrentWeapon->get_auto_reload())
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::RELOAD));
  }

  m_WeaponManager->_shoot_weapon(delta);

  if(m_WeaponStateContext.ShootTimeBeforeIdle <= 0.0f &&
     m_WeaponStateContext.IsKeyPressed == false && m_WeaponStateContext.IsKeyHeld == false)
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::IDLE));
  }
}

void WeaponShootState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Reload State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponReloadState::WeaponReloadState(const WeaponStateData& weaponStateData)
  : State(static_cast<int8_t>(WeaponStates::RELOAD)), m_WeaponManager(weaponStateData.weaponManager),
    m_WeaponStateMachine(weaponStateData.weaponStateMachine),
    m_WeaponStateContext(m_WeaponManager->get_weapon_state_ctx())
{
}

void WeaponReloadState::_handle_input(const Ref<InputEvent>& event)
{
}

void WeaponReloadState::_enter()
{
  if(m_WeaponManager == nullptr || m_WeaponStateMachine == nullptr)
  {
    print_error("Weapon Reload state data is null!");
    return;
  }
  
  m_CurrentWeapon = m_WeaponManager->get_current_weapon();
}

void WeaponReloadState::_update(double delta)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon") && m_WeaponStateContext.CurrentAnimTime >= m_CurrentWeapon->get_magEnteredTimestamp())
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::SHOOT));
  }
  
  m_WeaponManager->_reload_weapon();

  if(m_WeaponStateContext.IsReloading == false)
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::IDLE));
  }
}

void WeaponReloadState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Unequip State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponUnequipState::WeaponUnequipState(const WeaponStateData& weaponStateData)
  : State(static_cast<int8_t>(WeaponStates::UNEQUIP)), m_WeaponStateMachine(weaponStateData.weaponStateMachine),
    m_WeaponManager(weaponStateData.weaponManager), m_WeaponStateContext(m_WeaponManager->get_weapon_state_ctx())
{
}

void WeaponUnequipState::_handle_input(const Ref<InputEvent>& event)
{
  
}

void WeaponUnequipState::_enter()
{
  if(m_WeaponManager == nullptr || m_WeaponStateMachine == nullptr)
  {
    print_error("Weapon Unequip state data is null!");
    return;
  }

}

void WeaponUnequipState::_update(double delta)
{
  m_WeaponManager->_unequip_weapon();

  if(m_WeaponStateContext.IsUnequipped)
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::IDLE));
  }
}

void WeaponUnequipState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Switch State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponSwitchState::WeaponSwitchState(const WeaponStateData& weaponStateData)
  : State(static_cast<int8_t>(WeaponStates::WEAPON_SWITCH)), m_WeaponManager(weaponStateData.weaponManager),
    m_WeaponStateMachine(weaponStateData.weaponStateMachine)
{
}

void WeaponSwitchState::_handle_input(const Ref<InputEvent>& event)
{
 
}

void WeaponSwitchState::_enter()
{
  if(m_WeaponManager == nullptr || m_WeaponStateMachine == nullptr)
  {
    print_error("Weapon Switch state data is null!");
    return;
  }
  
  m_WeaponManager->_weapon_switch();
}

void WeaponSwitchState::_update(double delta)
{
  m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::EQUIP));
}

void WeaponSwitchState::_exit()
{

}
