#include "weapon_states.h"
#include "weapon_state_machine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Idle State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponIdleState::WeaponIdleState(WeaponStateMachine* weaponManager)
  : State(static_cast<int8_t>(WeaponStates::IDLE)), m_WeaponStateMachine(weaponManager)
{
}

void WeaponIdleState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::SHOOT));
  }

  if(Input::get_singleton()->is_action_just_pressed("unequip_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::UNEQUIP));
  }
  
}

void WeaponIdleState::_enter()
{
  if(m_WeaponStateMachine == nullptr)
  {
    print_error("Weapon Equip state data is null!");
    return;
  }

  m_WeaponComponent = m_WeaponStateMachine->get_weapon_component();
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
WeaponEquipState::WeaponEquipState(WeaponStateMachine* weaponStateMachine)
  : State(static_cast<int8_t>(WeaponStates::EQUIP)), m_WeaponStateMachine(weaponStateMachine)
{
}

void WeaponEquipState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon") && !m_WeaponAnimPlayer->is_playing())
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::SHOOT));
  }
}

void WeaponEquipState::_enter()
{
  m_CurrentWeapon = m_WeaponStateMachine->get_weapon_component()->get_current_weapon_data();
  
  m_WeaponAnimPlayer = m_WeaponStateMachine->get_current_weapon_anim_player();
  if(m_WeaponAnimPlayer == nullptr || !m_CurrentWeapon.is_valid())
  {
    print_error("Weapon Equip state data is null!");
    return;
  }

  m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName());
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
WeaponShootState::WeaponShootState(WeaponStateMachine* weaponManager)
  : State(static_cast<int8_t>(WeaponStates::SHOOT)), m_WeaponStateMachine(weaponManager)
{
}

void WeaponShootState::_handle_input(const Ref<InputEvent>& event)
{

  if(Input::get_singleton()->is_action_just_pressed("reload_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::RELOAD));
  }

  if(Input::get_singleton()->is_action_just_pressed("unequip_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::UNEQUIP));
  }
 
}

void WeaponShootState::_enter()
{
  m_CurrentWeapon = m_WeaponStateMachine->get_weapon_component()->get_current_weapon_data();
   
  m_WeaponAnimPlayer = m_WeaponStateMachine->get_current_weapon_anim_player();

  if(m_WeaponAnimPlayer == nullptr || !m_CurrentWeapon.is_valid())
  {
    print_error("Weapon Equip state data is null!");
    return;
  }

  m_ShootTimeBeforeIdle = m_WeaponStateMachine->get_shoot_time_before_idle();
  m_WantsToShoot = true;
}

void WeaponShootState::_update(double delta)
{
  m_WantsToShoot = false;
  if(m_ShootTimeBeforeIdle >= 0.0f)
  {
    m_ShootTimeBeforeIdle -= delta;
  }

  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon")) m_WantsToShoot = true;

  if(m_WantsToShoot == true)
  {
    m_WeaponAnimPlayer->stop(true);
    m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName());

    m_WantsToShoot = false;
  }

  if(m_ShootTimeBeforeIdle <= 0.0f)
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
WeaponReloadState::WeaponReloadState(WeaponStateMachine* weaponManager)
  : State(static_cast<int8_t>(WeaponStates::RELOAD)), m_WeaponStateMachine(weaponManager)
{
}

void WeaponReloadState::_handle_input(const Ref<InputEvent>& event)
{

}

void WeaponReloadState::_enter()
{
  m_CurrentWeapon = m_WeaponStateMachine->get_weapon_component()->get_current_weapon_data();

  m_WeaponAnimPlayer = m_WeaponStateMachine->get_current_weapon_anim_player();

  m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadAnimName());
}

void WeaponReloadState::_update(double delta)
{
  if(!m_WeaponAnimPlayer->is_playing())
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::IDLE));
}

void WeaponReloadState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Unequip State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponUnequipState::WeaponUnequipState(WeaponStateMachine* weaponManager)
  : State(static_cast<int8_t>(WeaponStates::UNEQUIP)), m_WeaponStateMachine(weaponManager)
{
}

void WeaponUnequipState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("equip_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::EQUIP));
  }
}

void WeaponUnequipState::_enter()
{
  m_CurrentWeapon = m_WeaponStateMachine->get_weapon_component()->get_current_weapon_data();
  m_WeaponComponent = m_WeaponStateMachine->get_weapon_component();

  m_WeaponAnimPlayer = m_WeaponStateMachine->get_current_weapon_anim_player();

  _unequip_weapon();
}

void WeaponUnequipState::_unequip_weapon()
{
  m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponUnequipAnimName());
}

void WeaponUnequipState::_update(double delta)
{
}

void WeaponUnequipState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Switch State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponSwitchState::WeaponSwitchState(WeaponStateMachine* weaponManager)
  : State(static_cast<int8_t>(WeaponStates::WEAPON_SWITCH)), m_WeaponStateMachine(weaponManager)
{
}

void WeaponSwitchState::_handle_input(const Ref<InputEvent>& event)
{
 
}

void WeaponSwitchState::_enter()
{
  m_CurrentWeapon = m_WeaponStateMachine->get_weapon_component()->get_current_weapon_data();
  m_WeaponComponent = m_WeaponStateMachine->get_weapon_component();

  m_WeaponAnimPlayer = m_WeaponStateMachine->get_current_weapon_anim_player();
  
  _weapon_switch();
}

void WeaponSwitchState::_weapon_switch()
{
  int weapon_index = -1;
  
  for (int i = 0; i < m_WeaponComponent->get_weapon_resource_list().size(); i++) {
    Ref<Weapon> res = m_WeaponComponent->get_weapon_resource_list()[i]; 

    if (res.is_valid() && res->get_weaponName() == m_WeaponComponent->get_next_weapon_name()) {
      weapon_index = i;
      break;
    }
  }
  
  if(weapon_index != -1)
  {
    Ref<Weapon> nextWeapon = m_WeaponComponent->get_weapon_resource_list()[weapon_index];
    m_WeaponComponent->set_current_weapon(nextWeapon);
  }
}

void WeaponSwitchState::_update(double delta)
{
  m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::EQUIP));
}

void WeaponSwitchState::_exit()
{

}