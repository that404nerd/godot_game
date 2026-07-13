#include "weapon_states.h"
#include "../weapon_manager.h"

BaseWeaponState::BaseWeaponState(WeaponStates movementState, const WeaponStateData& weaponStateData)
  : State(static_cast<int>(movementState)), m_WeaponManager(weaponStateData.weaponManager), m_WeaponStateMachine(weaponStateData.weaponStateMachine),
    m_WeaponStateContext(weaponStateData.weaponManager->get_weapon_state_ctx())
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Idle State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponIdleState::WeaponIdleState(const WeaponStateData& weaponStateData)
  : BaseWeaponState(WeaponStates::IDLE, weaponStateData)
{
}

void WeaponIdleState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WeaponManager->set_key_pressed(true);
    m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::SHOOT));
  }

  if(Input::get_singleton()->is_action_just_pressed("reload_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::RELOAD));
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
  : BaseWeaponState(WeaponStates::EQUIP, weaponStateData)
{
}

void WeaponEquipState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::SHOOT));
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
  m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::IDLE));
}


void WeaponEquipState::_exit()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Shoot State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponShootState::WeaponShootState(const WeaponStateData& weaponStateData)
  : BaseWeaponState(WeaponStates::SHOOT, weaponStateData)
{
}

void WeaponShootState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("reload_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::RELOAD));
  }

}

void WeaponShootState::_enter()
{
}

void WeaponShootState::_update(double delta)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon") && m_WeaponManager->get_current_weapon_ammo() == 0 && m_WeaponManager->current_weapon_has_auto_reload())
  {
    m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::RELOAD));

  } else if(m_WeaponStateContext.ShootTimeBeforeIdle <= 0.0f &&
            m_WeaponStateContext.IsKeyPressed == false && m_WeaponStateContext.IsKeyHeld == false)
  {
    m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::IDLE));
  }

  m_WeaponManager->_shoot_weapon(delta);

}

void WeaponShootState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Reload State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponReloadState::WeaponReloadState(const WeaponStateData& weaponStateData)
  : BaseWeaponState(WeaponStates::RELOAD, weaponStateData)
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
  
}

void WeaponReloadState::_update(double delta)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::SHOOT));
  }
  
  m_WeaponManager->_reload_weapon();

  if(m_WeaponStateContext.IsReloading == false)
  {
    m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::IDLE));
  }
}

void WeaponReloadState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Unequip State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponUnequipState::WeaponUnequipState(const WeaponStateData& weaponStateData)
  : BaseWeaponState(WeaponStates::UNEQUIP, weaponStateData)
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
    m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::IDLE));
  }
}

void WeaponUnequipState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Switch State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponSwitchState::WeaponSwitchState(const WeaponStateData& weaponStateData)
  : BaseWeaponState(WeaponStates::WEAPON_SWITCH, weaponStateData)
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
  m_WeaponStateMachine->_change_state(static_cast<int>(WeaponStates::EQUIP));
}

void WeaponSwitchState::_exit()
{

}
