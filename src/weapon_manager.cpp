#include "weapon_manager.h"
#include "player_state_machine.h"
#include "weapon_state_machine.h"

void WeaponManager::_ready()
{
}

void WeaponManager::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_bob_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_sway_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  GD_BIND_CUSTOM_PROPERTY(WeaponManager, hold_point_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, character_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
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
  if(character_state_machine && weapon_state_machine)
  {
    m_PlayerStateID = character_state_machine->get_current_state();
    m_WeaponStateID = weapon_state_machine->get_current_state();
  }
 
  weapon_bob_component->weapon_bob(delta);
  weapon_sway_component->weapon_sway(delta, m_MouseVel);

  if(m_PlayerStateID == static_cast<int8_t>(PlayerStates::IDLE))
  {
    if((Math::abs(m_MouseInput.x) <= 0.1f) && (Math::abs(m_MouseInput.y) <= 0.1f) && (m_WeaponStateID != static_cast<int8_t>(WeaponStates::SHOOT)))
    {
      weapon_sway_component->weapon_idle_sway(delta);
    }
  }

  m_MouseInput.x = 0.0f;
  m_MouseInput.y = 0.0f;
}

WeaponManager::~WeaponManager()
{
}