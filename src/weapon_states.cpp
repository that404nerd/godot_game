#include "weapon_states.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Idle State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponIdleState::WeaponIdleState(WeaponManager* weaponManager)
  : State(static_cast<uint8_t>(WeaponStates::IDLE)), m_WeaponManager(weaponManager)
{
}

void WeaponIdleState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot"))
  {
    m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::SHOOT));
  }
}

void WeaponIdleState::_enter()
{
  m_WeaponStateMachine = m_WeaponManager->get_weapon_state_machine();
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
WeaponEquipState::WeaponEquipState(WeaponManager* weaponManager)
  : State(static_cast<uint8_t>(WeaponStates::EQUIP)), m_WeaponManager(weaponManager)
{
}

void WeaponEquipState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot"))
  {
    m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::SHOOT));
  }
}

void WeaponEquipState::_enter()
{
  m_WeaponStateMachine = m_WeaponManager->get_weapon_state_machine();
<<<<<<< HEAD
  m_WeaponAnimPlayer = m_WeaponManager->get_weapon_anim_player();
  m_CurrentWeapon = m_WeaponManager->get_weapon_component().get_current_weapon_data();

=======
  m_CurrentWeapon = m_WeaponManager->get_weapon_component().get_current_weapon_data();

  Ref<PackedScene> weaponScene = m_CurrentWeapon->get_weaponScene();
  Node* weaponNode = weaponScene->instantiate();
  m_WeaponAnimPlayer = weaponNode->get_node<AnimationPlayer>(NodePath("%AnimationPlayer"));

>>>>>>> 55838bc (changed weapon anims)
  if(m_WeaponAnimPlayer == nullptr || !m_CurrentWeapon.is_valid())
  {
    assert("Weapon Equip state data is null!");
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
WeaponShootState::WeaponShootState(WeaponManager* weaponManager)
  : State(static_cast<uint8_t>(WeaponStates::SHOOT)), m_WeaponManager(weaponManager)
{
}

void WeaponShootState::_handle_input(const Ref<InputEvent>& event)
{
  if(Input::get_singleton()->is_action_just_pressed("shoot"))
  {
    m_WantsToShoot = true;
  }
<<<<<<< HEAD
=======

  if(Input::get_singleton()->is_action_just_pressed("reload_weapon"))
  {
    m_WeaponStateMachine->_change_state(static_cast<uint8_t>(WeaponStates::RELOAD));
  }
>>>>>>> 55838bc (changed weapon anims)
}

void WeaponShootState::_enter()
{
  m_WeaponStateMachine = m_WeaponManager->get_weapon_state_machine();
<<<<<<< HEAD
  m_WeaponAnimPlayer = m_WeaponManager->get_weapon_anim_player();
  m_CurrentWeapon = m_WeaponManager->get_weapon_component().get_current_weapon_data();

=======
  m_CurrentWeapon = m_WeaponManager->get_weapon_component().get_current_weapon_data();

  Ref<PackedScene> weaponScene = m_CurrentWeapon->get_weaponScene();
  Node* weaponNode = weaponScene->instantiate(PackedScene::GEN_EDIT_STATE_INSTANCE);
  m_WeaponAnimPlayer = weaponNode->get_node<AnimationPlayer>(NodePath("%AnimationPlayer"));

  print_line(m_WeaponAnimPlayer->get_animation_list());

>>>>>>> 55838bc (changed weapon anims)
  if(m_WeaponAnimPlayer == nullptr || !m_CurrentWeapon.is_valid())
  {
    assert("Weapon Equip state data is null!");
  }

  m_DidShoot = false;
<<<<<<< HEAD
  m_WantsToShoot = false;
=======
  m_WantsToShoot = true;
>>>>>>> 55838bc (changed weapon anims)
  m_TimeBetweenShots = 0.3f;
}

void WeaponShootState::_update(double delta)
{

<<<<<<< HEAD
  if(m_DidShoot == false && m_TimeBetweenShots > 0.0f)
=======
  if(m_DidShoot == false && m_WantsToShoot == true)
>>>>>>> 55838bc (changed weapon anims)
  {
    m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName());
    m_TimeBetweenShots -= delta;

    if(m_TimeBetweenShots <= 0.0f)
    {
      m_DidShoot = true;
      m_WantsToShoot = false;
    }
  }

<<<<<<< HEAD
  print_line("Time between shots: ", m_TimeBetweenShots);

=======
>>>>>>> 55838bc (changed weapon anims)
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

<<<<<<< HEAD
=======
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Weapon Reload State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
WeaponReloadState::WeaponReloadState(WeaponManager* weaponManager)
  : State(static_cast<uint8_t>(WeaponStates::RELOAD)), m_WeaponManager(weaponManager)
{
}

void WeaponReloadState::_handle_input(const Ref<InputEvent>& event)
{

}

void WeaponReloadState::_enter()
{
  m_WeaponStateMachine = m_WeaponManager->get_weapon_state_machine();
  m_CurrentWeapon = m_WeaponManager->get_weapon_component().get_current_weapon_data();

  Ref<PackedScene> weaponScene = m_CurrentWeapon->get_weaponScene();
  Node* weaponNode = weaponScene->instantiate();
  m_WeaponAnimPlayer = weaponNode->get_node<AnimationPlayer>(NodePath("%AnimationPlayer"));

  if(m_WeaponAnimPlayer == nullptr || !m_CurrentWeapon.is_valid())
  {
    assert("Weapon Equip state data is null!");
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
>>>>>>> 55838bc (changed weapon anims)
