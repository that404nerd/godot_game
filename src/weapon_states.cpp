#include "weapon_states.h"
#include "weapon_state_machine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Idle State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponIdleState::WeaponIdleState(WeaponStateMachine* weaponManager)
  : State(static_cast<uint8_t>(WeaponStates::IDLE)), m_WeaponStateMachine(weaponManager)
{
}

void WeaponIdleState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::SHOOT));
  }

  if(Input::get_singleton()->is_action_just_pressed("unequip_weapon") && m_IsUnequiped == false)
  {
    m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::UNEQUIP));
    m_IsUnequiped = true;
  }

  if(Input::get_singleton()->is_action_just_pressed("unequip_weapon") && m_IsUnequiped == true)
  {
    m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::EQUIP));
    m_IsUnequiped = false;
  }
}

void WeaponIdleState::_enter()
{
  if(m_WeaponStateMachine == nullptr)
  {
    assert("Weapon Equip state data is null!");
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
WeaponEquipState::WeaponEquipState(WeaponStateMachine* weaponStateMachine)
  : State(static_cast<uint8_t>(WeaponStates::EQUIP)), m_WeaponStateMachine(weaponStateMachine)
{
}

void WeaponEquipState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::SHOOT));
  }
}

void WeaponEquipState::_enter()
{
  m_CurrentWeapon = m_WeaponStateMachine->get_weapon_component()->get_current_weapon_data();
  
  CharacterBody3D* characterBody = m_WeaponStateMachine->get_character_component()->get_character_body();
  Array animPlayers = characterBody->get_tree()->get_nodes_in_group("weapon_anims");
  if (!animPlayers.is_empty()) {
    m_WeaponAnimPlayer = Object::cast_to<AnimationPlayer>(animPlayers[0]);
  }
 
  if(m_WeaponAnimPlayer == nullptr || !m_CurrentWeapon.is_valid())
  {
    print_error("Weapon Equip state data is null!");
    return;
  }
  
  m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName());
}

void WeaponEquipState::_update(double delta)
{
  m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::IDLE));
}


void WeaponEquipState::_exit()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Shoot State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponShootState::WeaponShootState(WeaponStateMachine* weaponManager)
  : State(static_cast<uint8_t>(WeaponStates::SHOOT)), m_WeaponStateMachine(weaponManager)
{
}

void WeaponShootState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon"))
  {
    m_WantsToShoot = true;
  }
  
  if(Input::get_singleton()->is_action_just_pressed("reload_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::RELOAD));
  }

  if(Input::get_singleton()->is_action_just_pressed("unequip_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::UNEQUIP));
  }
 
}

void WeaponShootState::_enter()
{
  m_CurrentWeapon = m_WeaponStateMachine->get_weapon_component()->get_current_weapon_data();
   
  CharacterBody3D* characterBody = m_WeaponStateMachine->get_character_component()->get_character_body();
  Array animPlayers = characterBody->get_tree()->get_nodes_in_group("weapon_anims");

  if (!animPlayers.is_empty()) {
    m_WeaponAnimPlayer = Object::cast_to<AnimationPlayer>(animPlayers[0]);
  }

  if(m_WeaponAnimPlayer == nullptr || !m_CurrentWeapon.is_valid())
  {
    print_error("Weapon Equip state data is null!");
    return;
  }

  m_DidShoot = false;
  m_WantsToShoot = true;
  m_TimeBetweenShots = 0.3f;
}

void WeaponShootState::_update(double delta)
{

  
  if(m_DidShoot == false && m_WantsToShoot == true)
  {
    m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName());
    m_TimeBetweenShots -= delta;

    if(m_TimeBetweenShots <= 0.0f)
    {
      m_DidShoot = true;
      m_WantsToShoot = false;
    }
  }

  
  if(m_DidShoot == true && m_WantsToShoot == true)
  {
    m_DidShoot = false;
    m_TimeBetweenShots = 0.3f;
  }

  
    // m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::IDLE));
}

void WeaponShootState::_exit()
{

}

  
   
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Reload State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponReloadState::WeaponReloadState(WeaponStateMachine* weaponManager)
  : State(static_cast<uint8_t>(WeaponStates::RELOAD)), m_WeaponStateMachine(weaponManager)
{
}

void WeaponReloadState::_handle_input(const Ref<InputEvent>& event)
{

}

void WeaponReloadState::_enter()
{
  m_CurrentWeapon = m_WeaponStateMachine->get_weapon_component()->get_current_weapon_data();

  CharacterBody3D* characterBody = m_WeaponStateMachine->get_character_component()->get_character_body();
  
  Array animPlayers = characterBody->get_tree()->get_nodes_in_group("weapon_anims");
  if (!animPlayers.is_empty()) {
    m_WeaponAnimPlayer = Object::cast_to<AnimationPlayer>(animPlayers[0]);
  }

  m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadAnimName());
}

void WeaponReloadState::_update(double delta)
{
  m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::IDLE));
}

void WeaponReloadState::_exit()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Unequip State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponUnequipState::WeaponUnequipState(WeaponStateMachine* weaponManager)
  : State(static_cast<uint8_t>(WeaponStates::UNEQUIP)), m_WeaponStateMachine(weaponManager)
{
}

void WeaponUnequipState::_handle_input(const Ref<InputEvent>& event)
{

}

void WeaponUnequipState::_enter()
{
  m_CurrentWeapon = m_WeaponStateMachine->get_weapon_component()->get_current_weapon_data();

  CharacterBody3D* characterBody = m_WeaponStateMachine->get_character_component()->get_character_body();
  
  Array animPlayers = characterBody->get_tree()->get_nodes_in_group("weapon_anims");
  if (!animPlayers.is_empty()) {
    m_WeaponAnimPlayer = Object::cast_to<AnimationPlayer>(animPlayers[0]);
  }

  m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponUnequipAnimName());
}

void WeaponUnequipState::_update(double delta)
{
  m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::IDLE));
}

void WeaponUnequipState::_exit()
{

}