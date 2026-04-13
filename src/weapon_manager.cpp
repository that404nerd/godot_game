#include "weapon_manager.h"

void WeaponManager::_bind_methods()
{
  ADD_SIGNAL(MethodInfo("weapon_equip"));
  ADD_SIGNAL(MethodInfo("weapon_unequip", PropertyInfo(Variant::STRING, "nextWeaponName")));
  ADD_SIGNAL(MethodInfo("weapon_shoot"));
  ADD_SIGNAL(MethodInfo("weapon_reload"));
  ADD_SIGNAL(MethodInfo("weapon_switch"));
  ADD_SIGNAL(MethodInfo("weapon_change", PropertyInfo(Variant::STRING, "nextWeaponName")));

  ADD_SIGNAL(MethodInfo("weapon_equip_finished"));
  ADD_SIGNAL(MethodInfo("weapon_unequip_finished"));
}

void WeaponManager::_init_data(CharacterComponent* characterComponent, Node3D* holdPoint, StateMachine* stateMachine)
{
  m_PlayerStateMachine = stateMachine;

  m_WeaponComponent.set_current_weapon(m_WeaponResourceList[0]);
  m_WeaponBobComponent._init_data({ characterComponent, &m_WeaponComponent, holdPoint });
  m_WeaponSwayComponent._init_data({ characterComponent, &m_WeaponComponent, holdPoint });

}

void WeaponManager::_ready()
{
  m_WeaponResourceList = {
    ResourceLoader::get_singleton()->load("res://assets/weapon_resources/pistol_resource.tres"),
    ResourceLoader::get_singleton()->load("res://assets/weapon_resources/rifle_resource.tres"),
    ResourceLoader::get_singleton()->load("res://assets/weapon_resources/shotgun_resource.tres"),
  };
}

void WeaponManager::_unhandled_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouseEvent = event;

  if(event->is_class("InputEventMouseMotion")) {
    m_MouseInput.x += -mouseEvent->get_screen_relative().x * 0.003f;
    m_MouseInput.y += -mouseEvent->get_screen_relative().y * 0.003f;
  }
}

void WeaponManager::_process(double delta)
{
  m_CurrentStateName = m_PlayerStateMachine->get_current_state();
 
  if(m_CurrentStateName == StringName("Sprint"))
  {
    m_WeaponBobComponent.weapon_bob(delta);
  }
  
  if(m_CurrentStateName == StringName("Idle"))
  {
    /* NOTE: Didn't use is_zero_approx() because the EPSILON Value was a little too low and transitioning from normal sway 
     * to idle sway causes the weapon to do a little snap (tbh it's barely noticable but it's kinda annoying me), so checking with 0.1f instead.
     */
    if((Math::abs(m_MouseInput.x) < 0.1f) && (Math::abs(m_MouseInput.y) < 0.1f))
    {
      m_WeaponSwayComponent.weapon_idle_sway(delta);
    } else {
      m_WeaponSwayComponent.weapon_sway(delta, m_MouseInput);
    }
  
  }
  
  m_WeaponSwayComponent.reset_weapon_sway(delta);

  m_MouseInput.x = Math::lerp(m_MouseInput.x, 0.0f, MOUSE_INPUT_RESET_MULTIPLIER * (float)delta);
  m_MouseInput.y = Math::lerp(m_MouseInput.y, 0.0f, MOUSE_INPUT_RESET_MULTIPLIER * (float)delta);
}
