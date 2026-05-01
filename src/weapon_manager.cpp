#include "weapon_manager.h"
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
    anim_player->connect("animation_finished", Callable(this, "_on_animation_finished"));
  }
  
  m_CurrentWeaponAnimPlayer = Object::cast_to<AnimationPlayer>(m_WeaponAnimGroups[m_WeaponIndex]);
  // Set the current weapon right here first!
  m_CurrentWeapon = weapon_component->get_current_weapon_data();
  
}

void WeaponManager::_bind_methods()
{
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

  // weapon_bob_component->weapon_bob_up(delta);
  m_WeaponEffects._update(delta, m_MouseVel);

  m_MouseInput.x = 0.0f;
  m_MouseInput.y = 0.0f;
}

void WeaponManager::_equip_weapon()
{
  m_CurrentWeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName(), 
                           m_CurrentWeapon->get_weapon_equip_anim_blend(), m_CurrentWeapon->get_weapon_equip_anim_speed());
}

void WeaponManager::_unequip_weapon()
{

}

void WeaponManager::_shoot_weapon()
{

}

void WeaponManager::_reload_weapon()
{

}

void WeaponManager::_weapon_unequip_over()
{

}

void WeaponManager::_weapon_switch()
{

}

void WeaponManager::_switch_weapon_data()
{

}

WeaponManager::~WeaponManager()
{
}