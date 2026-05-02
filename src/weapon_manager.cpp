#include "weapon_manager.h"
#include "weapon_states.h"
#include "weapon_state_machine.h"

void WeaponManager::_ready()
{
  m_WeaponNodesGroup = get_tree()->get_nodes_in_group(StringName("weapon_nodes"));
  m_WeaponAnimGroups = get_tree()->get_nodes_in_group(StringName("weapon_anims"));
  
  weapon_component->set_current_weapon(weapon_component->get_weapon_resource_list()[m_WeaponIndex]);
  m_WeaponEffects._init_data(hold_point_node, character_component, weapon_component);

  /* NOTE: This took me 2 hours to find lol, i forgot that i was dealing with different animation
  players for different weapon scene, this connects the _on_animation_finished to all animation players */
  for(int i = 0; i < m_WeaponAnimGroups.size(); i++)
  {
    AnimationPlayer* anim_player = Object::cast_to<AnimationPlayer>(m_WeaponAnimGroups[i]);
    anim_player->connect("animation_finished", Callable(weapon_state_machine, "_on_animation_finished"));
  }
  
  m_CurrentWeaponAnimPlayer = Object::cast_to<AnimationPlayer>(m_WeaponAnimGroups[m_WeaponIndex]);
  // Set the current weapon right here first!
  m_CurrentWeapon = weapon_component->get_current_weapon_data();

}

void WeaponManager::_bind_methods()
{
  
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, hold_point_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
 
}

void WeaponManager::_unhandled_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouseEvent = event;
  
  if(event->is_class("InputEventMouseMotion")) {
    float swayIntensity = 0.005f; 

    m_MouseInput.x += -mouseEvent->get_screen_relative().x * 0.003f;
    m_MouseInput.y += -mouseEvent->get_screen_relative().y * 0.003f;

    Vector2 relative = mouseEvent->get_relative(); 
    m_MouseVel.x += -relative.x * swayIntensity;
    m_MouseVel.y += -relative.y * swayIntensity;
  }
}

void WeaponManager::_process(double delta)
{
  m_CurrentWeapon = weapon_component->get_current_weapon_data();
  m_WeaponStateCtx.CurrentWeaponType = m_CurrentWeapon->get_weapon_type();

  m_WeaponEffects._update(delta, m_MouseVel);
  
  m_MouseInput.x = 0.0f;
  m_MouseInput.y = 0.0f;
}


///////////////////////////////////////////////////////////////////////
/////////////////// Weapon State Implementation ///////////////////////
///////////////////////////////////////////////////////////////////////
void WeaponManager::_equip_weapon()
{
  m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName(), 
                           m_CurrentWeapon->get_weapon_equip_anim_blend(), m_CurrentWeapon->get_weapon_equip_anim_speed());
}

void WeaponManager::_unequip_weapon()
{
  if(weapon_component->get_next_weapon_name() != m_CurrentWeapon->get_weaponName())
  {
    if(m_CurrentWeaponAnimPlayer->get_current_animation() != m_CurrentWeapon->get_weaponUnequipAnimName())
    {
      m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponUnequipAnimName(), 
                            m_CurrentWeapon->get_weapon_unequip_anim_blend(), m_CurrentWeapon->get_weapon_unequip_anim_speed());
    }
  }
  
}

void WeaponManager::_shoot_weapon(double delta)
{
  m_WeaponStateCtx.WantsToShoot = false;

  if(m_WeaponStateCtx.ShootTimeBeforeIdle >= 0.0f)
  {
    m_WeaponStateCtx.ShootTimeBeforeIdle -= delta;
  }

  if(m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::AUTO && 
    Input::get_singleton()->is_action_pressed("shoot_weapon"))
  {
    m_WeaponStateCtx.IsKeyHeld = true;
    m_WeaponStateCtx.ShootTimeBeforeIdle = 1.0f;

  }

  if(Input::get_singleton()->is_action_just_pressed("shoot_weapon") && 
    m_WeaponStateCtx.CurrentWeaponType == Weapon::WeaponType::MANUAL)
  {
    m_WeaponStateCtx.IsKeyHeld = false;
    m_WeaponStateCtx.WantsToShoot = true;
  }

  if(m_WeaponStateCtx.IsKeyHeld || m_WeaponStateCtx.WantsToShoot)
  {
    // if(m_AmmoComponent->get_current_weapon_ammo() <= 0)
    // {
    //   print_line("Reload!!");
    //   m_WeaponStateMachine->_change_state(static_cast<int8_t>(WeaponStates::RELOAD));
    // }
    // else
    // {
      // }
    m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName(), 
      m_CurrentWeapon->get_weapon_shoot_anim_blend(), m_CurrentWeapon->get_weapon_shoot_anim_speed());
  }

  if(!Input::get_singleton()->is_action_just_released("shoot_weapon"))
  {
    m_WeaponStateCtx.IsKeyHeld = false;
  }
}

void WeaponManager::_reload_weapon()
{
  m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadAnimName(), 
                      m_CurrentWeapon->get_weapon_reload_anim_blend(), m_CurrentWeapon->get_weapon_reload_anim_speed());
}

void WeaponManager::_weapon_unequip_over()
{
  m_CurrentWeaponAnimPlayer = Object::cast_to<AnimationPlayer>(m_WeaponAnimGroups[m_WeaponIndex]);
  weapon_component->set_current_weapon(weapon_component->get_next_weapon_data());
}

void WeaponManager::_weapon_switch()
{
  int weapon_index = -1;
  
  for (int i = 0; i < weapon_component->get_weapon_resource_list().size(); i++) {
    Ref<Weapon> res = weapon_component->get_weapon_resource_list()[i]; 

    if (res.is_valid() && res->get_weaponName() == weapon_component->get_next_weapon_name()) {
      weapon_index = i;
      break;
    }
  }
  
  if(weapon_index != -1)
  {
    Ref<Weapon> nextWeapon = weapon_component->get_weapon_resource_list()[weapon_index];
    weapon_component->set_current_weapon(nextWeapon);
  }
}

void WeaponManager::_switch_weapon_data(int weaponIndex)
{
  // We are still in the previous weapon and didn't switch to the next one yet
  Ref<Weapon> next_weapon = weapon_component->get_weapon_resource_list()[weaponIndex];
  
  weapon_component->set_next_weapon(next_weapon);
  weapon_component->set_next_weapon_name(next_weapon->get_weaponName());
  m_WeaponIndex = weaponIndex;
}

WeaponManager::~WeaponManager()
{
}