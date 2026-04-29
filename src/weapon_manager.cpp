#include "weapon_manager.h"
#include "player_state_machine.h"
#include "weapon_state_machine.h"

void WeaponManager::_ready()
{
  m_WeaponNodesGroup = get_tree()->get_nodes_in_group(StringName("weapon_nodes"));
}

void WeaponManager::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_bob_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(WeaponManager, weapon_sway_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
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
  if(weapon_state_machine)
  {
    m_WeaponStateID = weapon_state_machine->get_current_state();
  }
 
  weapon_bob_component->weapon_bob(delta);
  // weapon_bob_component->weapon_bob_up(delta);
  weapon_sway_component->weapon_sway(delta, m_MouseVel);

  if(m_WeaponStateID != static_cast<int8_t>(WeaponStates::SHOOT))
  {
    weapon_sway_component->weapon_idle_sway(delta);
  }

  m_MouseInput.x = 0.0f;
  m_MouseInput.y = 0.0f;
}

WeaponManager::~WeaponManager()
{
}