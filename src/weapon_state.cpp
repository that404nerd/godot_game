#include "weapon_state.h"

WeaponState::WeaponState()
{
}

void WeaponState::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponState, weapon_manager, Variant::OBJECT, PropertyHint::PROPERTY_HINT_NODE_TYPE);
  ClassDB::bind_method(D_METHOD("_on_animation_finished", "anim_name"), &WeaponState::_on_animation_finished);
  ClassDB::bind_method(D_METHOD("_on_weapon_equip_finished"), &WeaponState::_on_weapon_equip_anim_finished);
  ClassDB::bind_method(D_METHOD("_on_weapon_unequip_finished"), &WeaponState::_on_weapon_unequip_anim_finished);
}

void WeaponState::_input(const Ref<InputEvent>& event)
{
  // for(int i = 0; i < weapon_manager->get_weaponList().size(); i++)
  // {
  //   String inputAction = "weapon_" + String::num(i + 1, 0); // INFO: Need to match the set input action in the editor
  //   if(Input::get_singleton()->is_action_just_pressed(inputAction))
  //   {
  //     Ref<Weapon> next_weapon = weapon_manager->get_weaponList()[i];
  //     String next_weapon_name = next_weapon->get_weaponName();
  //
  //     weapon_manager->set_weapon_state(WeaponStates::UNEQUIP, { nullptr, next_weapon_name, i });
  //   }
  // }
  //
}

void WeaponState::_ready()
{
  // m_WeaponAnimPlayer = weapon_manager->get_weapon_anim_player();
  // if(m_WeaponAnimPlayer && weapon_manager)
  // {
  //   m_WeaponAnimPlayer->connect("animation_finished", Callable(this, "_on_animation_finished"));   
  //   weapon_manager->connect("weapon_equip_finished", Callable(this, "_on_weapon_equip_anim_finished"));   
  //   weapon_manager->connect("weapon_unequip_finished", Callable(this, "_on_weapon_unequip_anim_finished"));   
  // }
  //
  // m_CurrentWeapon = weapon_manager->get_current_weapon();
  // m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName());
}

void WeaponState::_weapon_shoot()
{
  // if(weapon_manager->get_ammo_count() > 0)
  // // {
  //   weapon_manager->set_weapon_state(WeaponStates::SHOOT);
  // m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName());
  // // } else return;
}

void WeaponState::_weapon_reload()
{
  // if((m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount != m_CurrentWeapon->get_totalAmmoCount() || m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount == 0))
  // {
  //     // NOTE: Currently no ammo pickups...
  //   weapon_manager->set_weapon_state(WeaponStates::RELOAD);
  // m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadAnimName());
  // }
}

void WeaponState::_weapon_change()
{
  // int weapon_index = -1;
  //
  // for (int i = 0; i < weapon_manager->get_weaponList().size(); i++) {
  //   Ref<Weapon> res = weapon_manager->get_weaponList()[i]; 
  //
  //   if (res.is_valid() && res->get_weaponName() == weapon_manager->get_next_weapon_name()) {
  //
  //     print_line("Weapon found at index: ", i);
  //     weapon_index = i;
  //     break;
  //   }
  // }
  //
  // if(weapon_index != -1)
  // {
  //   Ref<Weapon> wep = weapon_manager->get_weaponList()[weapon_index];
  //   print_line("New Weapon name is: ", wep->get_weaponEquipAnimName());
  //   weapon_manager->set_weapon_state(WeaponStates::EQUIP, { weapon_manager->get_weaponList()[weapon_index], "", weapon_index });
  // }
}

void WeaponState::_on_animation_finished(const StringName& anim_name)
{
  // if(anim_name == m_CurrentWeapon->get_weaponUnequipAnimName())
  // {
  //   print_line("Weapon unequip animation is over ", anim_name);
  //   weapon_manager->set_weapon_state(WeaponStates::WEAPON_SWITCH, { nullptr, "", -1 });
  // }
  //
  // if(anim_name == m_CurrentWeapon->get_weaponEquipAnimName())
  // {
  //   print_line("Weapon equip animation is over ", anim_name);
  //   weapon_manager->set_weapon_state(WeaponStates::IDLE);
  // }
}

void WeaponState::_on_weapon_equip_anim_finished()
{
  // weapon_manager->set_weapon_state(WeaponStates::IDLE);
}

void WeaponState::_on_weapon_unequip_anim_finished()
{
  // weapon_manager->set_weapon_state(WeaponStates::IDLE);
}

void WeaponState::_weapon_unequip()
{
  // if(weapon_manager->get_next_weapon_name() != m_CurrentWeapon->get_weaponName())
  // {
  //   if(m_WeaponAnimPlayer->get_current_animation() != m_CurrentWeapon->get_weaponUnequipAnimName())
  //   {
  //     m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponUnequipAnimName());
  //
  //     print_line("Unequipped weapon name is: ", weapon_manager->get_current_weapon()->get_weaponName());
  //     weapon_manager->set_weapon_state(WeaponStates::NONE, { nullptr, weapon_manager->get_next_weapon_name(), -1 });
  //   }
  // }
}

void WeaponState::_weapon_equip()
{
  // print_line("Current equip anim is: ", m_CurrentWeapon->get_weaponEquipAnimName());
  // m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName());
}

void WeaponState::_process_states()
{
  // switch(weapon_manager->get_current_weapon_state())
  // {
  //   /* NOTE: This is so that the weapon manager will take care if the weapon is idle,
  //      because this class doesn't care about swaying, bobbing and other effects which depend on player movement */
  //   case WeaponStates::IDLE:
  //     break;
  //   case WeaponStates::WEAPON_SWITCH:
  //     _weapon_change();
  //     break; // TODO: Implement this later
  //   case WeaponStates::EQUIP:
  //     _weapon_equip();
  //     break;
  //   case WeaponStates::RELOAD:
  //     _weapon_reload();
  //     break;
  //   case WeaponStates::SHOOT:
  //     _weapon_shoot();
  //     break;
  //   case WeaponStates::UNEQUIP:
  //     _weapon_unequip();
  //     break;
  //   case WeaponStates::NONE:
  //     break;
  // }
}

void WeaponState::_physics_process(double delta)
{
  // _process_states();

  // if(Input::get_singleton()->is_action_just_pressed("shoot"))
  //   weapon_manager->set_weapon_state(WeaponStates::SHOOT);
}
