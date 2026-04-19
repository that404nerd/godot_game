#include "weapon_manager.h"
#include "player_state_machine.h"
#include "weapon_state_machine.h"

void WeaponManager::_init_data(const WeaponManagerData& weaponManagerData)
{
  if(weaponManagerData.characterComponent == nullptr || weaponManagerData.holdPoint == nullptr 
    || weaponManagerData.stateMachine == nullptr)
  {
    print_error("WeaponManagerData is uninitialized!");
    return;
  }

  m_CharacterStateMachine = weaponManagerData.stateMachine;
  m_WeaponStateMachine = memnew(WeaponStateMachine);

  m_WeaponResourceList = {
    ResourceLoader::get_singleton()->load("res://assets/weapon_resources/pistol_resource.tres"),
    ResourceLoader::get_singleton()->load("res://assets/weapon_resources/rifle_resource.tres"),
    ResourceLoader::get_singleton()->load("res://assets/weapon_resources/shotgun_resource.tres"),
  };

  m_WeaponComponent.set_current_weapon(m_WeaponResourceList[0]);

  m_WeaponStateMachine->_init_data({ this, &m_WeaponComponent, weaponManagerData.characterComponent });
  m_WeaponBobComponent._init_data({ weaponManagerData.characterComponent, m_WeaponComponent, weaponManagerData.holdPoint });
  m_WeaponSwayComponent._init_data({ weaponManagerData.characterComponent, m_WeaponComponent, weaponManagerData.holdPoint });
}

void WeaponManager::_ready()
{
  add_child(m_WeaponStateMachine);
}

void WeaponManager::_bind_methods()
{
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
  m_CurrentStateID = m_CharacterStateMachine->get_current_state();
 
  if(m_CurrentStateID == static_cast<uint8_t>(PlayerStates::SPRINT))
  {
    m_WeaponBobComponent.weapon_bob(delta);
  }
  
  if(m_CurrentStateID == static_cast<uint8_t>(PlayerStates::IDLE))
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

WeaponManager::~WeaponManager()
{
}
