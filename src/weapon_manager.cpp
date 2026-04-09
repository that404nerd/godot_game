#include "weapon_manager.h"

WeaponManager::WeaponManager()
{
}

void WeaponManager::_bind_methods()
{
  GD_BIND_PROPERTY(WeaponManager, weaponList, Variant::ARRAY);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_bob_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_sway_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ADD_SIGNAL(MethodInfo("weapon_equip"));
  ADD_SIGNAL(MethodInfo("weapon_unequip", PropertyInfo(Variant::STRING, "nextWeaponName")));
  ADD_SIGNAL(MethodInfo("weapon_shoot"));
  ADD_SIGNAL(MethodInfo("weapon_reload"));
  ADD_SIGNAL(MethodInfo("weapon_switch"));
  ADD_SIGNAL(MethodInfo("weapon_change", PropertyInfo(Variant::STRING, "nextWeaponName")));

  ADD_SIGNAL(MethodInfo("weapon_equip_finished"));
  ADD_SIGNAL(MethodInfo("weapon_unequip_finished"));
}

void WeaponManager::_ready()
{
  weapon_component->set_current_weapon(weaponList[0]);
}

void WeaponManager::_unhandled_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouseEvent = event;

  if(event->is_class("InputEventMouseMotion")) {
    m_MouseInput.x += -mouseEvent->get_screen_relative().x * 0.003f;
    m_MouseInput.y += -mouseEvent->get_screen_relative().y * 0.003f;

    if(!mouseEvent->get_relative().is_zero_approx())
    {
      m_IsMovingMouse = true;
    } else if(mouseEvent->get_relative().is_zero_approx()) {
      m_IsMovingMouse = false;
    }
  }
}

void WeaponManager::_process(double delta)
{
  // YUCK...
  m_CurrentStateName = GameManager::get_singleton()->get_player_state_machine()->get_current_state();

  if(weapon_bob_component && m_CurrentStateName == StringName("Sprint"))
  {
    print_line("Weapon Bobbing!"); 
    weapon_bob_component->weapon_bob(delta);
  }

  if(weapon_component && m_CurrentStateName == StringName("Idle"))
  {
    if(m_IsMovingMouse == true)
    {
      weapon_sway_component->weapon_sway(m_MouseInput);
      print_line("Swaying weapon!");
    }
    if(m_IsMovingMouse == false)
    {
      weapon_sway_component->weapon_idle_sway(delta);
      print_line("Idle swaying weapon!");
    }
  }
}

WeaponManager::~WeaponManager()
{
}
