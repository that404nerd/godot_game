#include "weapon_states.h"
#include "weapon_state_machine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Idle State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponIdleState::WeaponIdleState(const WeaponStateData& weaponStateData)
  : State(static_cast<int8_t>(WeaponStates::IDLE)), m_WeaponManager(weaponStateData.weaponManager), 
    m_WeaponStateMachine(weaponStateData.weaponStateMachine)
{
}

void WeaponIdleState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::SHOOT));
  }

  if(Input::get_singleton()->is_action_just_pressed("reload_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::RELOAD));
  }
  
}

void WeaponIdleState::_enter()
{
  // if(m_WeaponManager == nullptr)
  // {
  //   print_error("Weapon Equip state data is null!");
  //   return;
  // }

  // m_WeaponComponent = m_WeaponManager->get_weapon_component();
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
  // m_AmmoComponent = m_WeaponManager->get_ammo_component();
  // m_CurrentWeapon = m_WeaponManager->get_weapon_component()->get_current_weapon_data();
  
  // m_WeaponAnimPlayer = m_WeaponManager->get_current_weapon_anim_player();

  // if(m_WeaponAnimPlayer == nullptr || !m_CurrentWeapon.is_valid())
  // {
  //   print_error("Weapon Equip state data is null!");
  //   return;
  // }

  // // this sets the total ammo of the current equipped weapon
  // m_AmmoComponent->set_total_weapon_ammo(m_CurrentWeapon);

  // m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName(), 
  //           m_CurrentWeapon->get_weapon_equip_anim_blend(), m_CurrentWeapon->get_weapon_equip_anim_speed());

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
    m_WeaponStateMachine(weaponStateData.weaponStateMachine)
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
  // m_CurrentWeapon = m_WeaponManager->get_weapon_component()->get_current_weapon_data();
  // m_AmmoComponent = m_WeaponManager->get_ammo_component(); 
  // m_WeaponAnimPlayer = m_WeaponManager->get_current_weapon_anim_player();

  // if(m_WeaponAnimPlayer == nullptr || !m_CurrentWeapon.is_valid())
  // {
  //   print_error("Weapon Equip state data is null!");
  //   return;
  // }

  // m_ShootTimeBeforeIdle = m_WeaponManager->get_shoot_time_before_idle();
  // m_WeaponType = m_CurrentWeapon->get_weapon_type();
  // m_WantsToShoot = true;
  // m_IsKeyHeld = true;
}

void WeaponShootState::_update(double delta)
{
  m_WeaponManager->_shoot_weapon();
  // m_WantsToShoot = false;

  // if(m_ShootTimeBeforeIdle >= 0.0f)
  // {
  //   m_ShootTimeBeforeIdle -= delta;
  // }

  // if(m_WeaponType == Weapon::WeaponType::AUTO && Input::get_singleton()->is_action_pressed("shoot_weapon"))
  // {
  //   m_IsKeyHeld = true;
  //   m_ShootTimeBeforeIdle = m_WeaponManager->get_shoot_time_before_idle();
  // }

  // if(Input::get_singleton()->is_action_just_pressed("shoot_weapon") && m_WeaponType == Weapon::WeaponType::MANUAL)
  // {
  //   m_IsKeyHeld = false;
  //   m_WantsToShoot = true;
  // }

  
  // if(m_WantsToShoot || m_IsKeyHeld)
  // {
  //   if(m_AmmoComponent->get_current_weapon_ammo() <= 0)
  //   {
  //     print_line("Reload!!");
  //     m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::RELOAD));
  //   }
  //   else
  //   {
  //     m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName(), m_CurrentWeapon->get_weapon_shoot_anim_blend(), m_CurrentWeapon->get_weapon_shoot_anim_speed());
  //   }
  // }


  // if(!Input::get_singleton()->is_action_just_released("shoot_weapon"))
  // {
  //   m_IsKeyHeld = false;
  // }

  // if(m_ShootTimeBeforeIdle <= 0.0f && m_IsKeyHeld == false && m_WantsToShoot == false)
  // {
  //   m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::IDLE));
  // }

}

void WeaponShootState::_exit()
{

}

  
   
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Reload State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponReloadState::WeaponReloadState(const WeaponStateData& weaponStateData)
  : State(static_cast<int8_t>(WeaponStates::RELOAD)), m_WeaponManager(weaponStateData.weaponManager),
    m_WeaponStateMachine(weaponStateData.weaponStateMachine)
{
}

void WeaponReloadState::_handle_input(const Ref<InputEvent>& event)
{

}

void WeaponReloadState::_enter()
{
  // m_CurrentWeapon = m_WeaponManager->get_weapon_component()->get_current_weapon_data();
  // m_AmmoComponent = m_WeaponManager->get_ammo_component();

  // m_WeaponAnimPlayer = m_WeaponManager->get_current_weapon_anim_player();

  // m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadAnimName(), 
  //                     m_CurrentWeapon->get_weapon_reload_anim_blend(), m_CurrentWeapon->get_weapon_reload_anim_speed());

  // m_AmmoComponent->set_current_weapon_ammo(m_CurrentWeapon);

  m_WeaponManager->_reload_weapon();
}

void WeaponReloadState::_update(double delta)
{
  m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::IDLE));
}

void WeaponReloadState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Unequip State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponUnequipState::WeaponUnequipState(const WeaponStateData& weaponStateData)
  : State(static_cast<int8_t>(WeaponStates::UNEQUIP)), m_WeaponStateMachine(weaponStateData.weaponStateMachine),
    m_WeaponManager(weaponStateData.weaponManager)
{
}

void WeaponUnequipState::_handle_input(const Ref<InputEvent>& event)
{
  
}

void WeaponUnequipState::_enter()
{
  // m_CurrentWeapon = m_WeaponManager->get_weapon_component()->get_current_weapon_data();
  // m_WeaponComponent = m_WeaponManager->get_weapon_component();

  // m_WeaponAnimPlayer = m_WeaponManager->get_current_weapon_anim_player();

  m_WeaponManager->_unequip_weapon();
}

void WeaponUnequipState::_unequip_weapon()
{
  // if(m_WeaponComponent->get_next_weapon_name() != m_CurrentWeapon->get_weaponName())
  // {
  //   if(m_WeaponAnimPlayer->get_current_animation() != m_CurrentWeapon->get_weaponUnequipAnimName())
  //   {
  //     m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponUnequipAnimName(), 
  //                           m_CurrentWeapon->get_weapon_unequip_anim_blend(), m_CurrentWeapon->get_weapon_unequip_anim_speed());
  //   }
  // }
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
  // m_CurrentWeapon = m_WeaponManager->get_weapon_component()->get_current_weapon_data();
  // m_WeaponComponent = m_WeaponManager->get_weapon_component();

  // m_WeaponAnimPlayer = m_WeaponManager->get_current_weapon_anim_player();
  
  m_WeaponManager->_weapon_switch();
}

void WeaponSwitchState::_weapon_switch()
{
  // int weapon_index = -1;
  
  // for (int i = 0; i < m_WeaponComponent->get_weapon_resource_list().size(); i++) {
  //   Ref<Weapon> res = m_WeaponComponent->get_weapon_resource_list()[i]; 

  //   if (res.is_valid() && res->get_weaponName() == m_WeaponComponent->get_next_weapon_name()) {
  //     weapon_index = i;
  //     break;
  //   }
  // }
  
  // if(weapon_index != -1)
  // {
  //   Ref<Weapon> nextWeapon = m_WeaponComponent->get_weapon_resource_list()[weapon_index];
  //   m_WeaponComponent->set_current_weapon(nextWeapon);
  // }
}

void WeaponSwitchState::_update(double delta)
{
  m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::EQUIP));
}

void WeaponSwitchState::_exit()
{

}