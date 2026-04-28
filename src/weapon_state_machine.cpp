#include "weapon_state_machine.h"
#include "weapon_states.h"

void WeaponStateMachine::_init_data()
{
  m_States[static_cast<int8_t>(WeaponStates::IDLE)] = std::make_unique<WeaponIdleState>(this);
  m_States[static_cast<int8_t>(WeaponStates::EQUIP)] = std::make_unique<WeaponEquipState>(this);
  m_States[static_cast<int8_t>(WeaponStates::SHOOT)] = std::make_unique<WeaponShootState>(this);
  m_States[static_cast<int8_t>(WeaponStates::RELOAD)] = std::make_unique<WeaponReloadState>(this);
  m_States[static_cast<int8_t>(WeaponStates::UNEQUIP)] = std::make_unique<WeaponUnequipState>(this);
  m_States[static_cast<int8_t>(WeaponStates::WEAPON_SWITCH)] = std::make_unique<WeaponSwitchState>(this);
  
  m_InitialState = m_States.at(static_cast<int8_t>(WeaponStates::EQUIP)).get();
  
  CharacterBody3D* characterBody = character_component->get_character_body();
  m_WeaponAnimGroups = characterBody->get_tree()->get_nodes_in_group("weapon_anims");

  /* NOTE: This took me 2 hours to find lol, i forgot that i was dealing with different animation
     players for different weapon scene, this connects the _on_animation_finished to all animation players */
  for(int i = 0; i < m_WeaponAnimGroups.size(); i++)
  {
    AnimationPlayer* anim_player = Object::cast_to<AnimationPlayer>(m_WeaponAnimGroups[i]);
    anim_player->connect("animation_finished", Callable(this, "_on_animation_finished"));
  }

  m_CurrentWeaponAnimPlayer = Object::cast_to<AnimationPlayer>(m_WeaponAnimGroups[m_WeaponIndex]);
  weapon_component->set_current_weapon(weapon_component->get_weapon_resource_list()[m_WeaponIndex]);
}

void WeaponStateMachine::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponStateMachine, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponStateMachine, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_PROPERTY(WeaponStateMachine, shoot_time_before_idle, Variant::FLOAT);

  ClassDB::bind_method(D_METHOD("get_current_state_name"), &WeaponStateMachine::get_current_state_name);
  ClassDB::bind_method(D_METHOD("_on_animation_finished", "anim_name"), &WeaponStateMachine::_on_animation_finished);
}

AnimationPlayer* WeaponStateMachine::get_current_weapon_anim_player()
{
  if(m_CurrentWeaponAnimPlayer)
  {
    return m_CurrentWeaponAnimPlayer;
  }

  print_error("Current weapon anim player is null!");
  return nullptr;
}

void WeaponStateMachine::_handle_state_machine_input(const Ref<InputEvent>& event)
{
  for(int i = 0; i < weapon_component->get_weapon_resource_list().size(); i++)
  {
    String inputAction = "weapon_" + String::num(i + 1, 0); // INFO: Need to match the set input action in the editor
    if(Input::get_singleton()->is_action_just_pressed(inputAction))
    {
      // We are still in the previous weapon and didn't switch to the next one yet
      Ref<Weapon> next_weapon = weapon_component->get_weapon_resource_list()[i];
  
      // Set the next weapon resource and name
      weapon_component->set_next_weapon(next_weapon);
      weapon_component->set_next_weapon_name(next_weapon->get_weaponName());
      m_WeaponIndex = i;
      
      _change_state(static_cast<int8_t>(WeaponStates::UNEQUIP));
    }
  }
}

void WeaponStateMachine::_on_animation_finished(const StringName& anim_name)
{
  if(anim_name == weapon_component->get_current_weapon_data()->get_weaponUnequipAnimName())
  {
    m_CurrentWeaponAnimPlayer = Object::cast_to<AnimationPlayer>(m_WeaponAnimGroups[m_WeaponIndex]);
    weapon_component->set_current_weapon(weapon_component->get_next_weapon_data());
    _change_state(static_cast<uint8_t>(WeaponStates::WEAPON_SWITCH));
  }
}