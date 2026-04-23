#include "weapon_manager.h"
#include "player_state_machine.h"
#include "weapon_state_machine.h"

void WeaponManager::_ready()
{
}

void WeaponManager::_bind_methods()
{
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
    m_MouseInput.x += -mouseEvent->get_screen_relative().x * 0.003f;
    m_MouseInput.y += -mouseEvent->get_screen_relative().y * 0.003f;
    
    float swayIntensity = 0.005f; 

    Vector2 relative = mouseEvent->get_relative(); 
    m_MouseVel.x += -relative.x * swayIntensity;
    m_MouseVel.y += -relative.y * swayIntensity;
  }
}

void WeaponManager::_process(double delta)
{
  m_CurrentStateID = character_state_machine->get_current_state();
 
  if(m_CurrentStateID == static_cast<int8_t>(PlayerStates::SPRINT))
  {
    weapon_bob_component->weapon_bob(delta);
  }
  
  if(m_CurrentStateID == static_cast<int8_t>(PlayerStates::IDLE))
  {
    /* NOTE: Didn't use is_zero_approx() because the EPSILON Value was a little too low and transitioning from normal sway 
     * to idle sway causes the weapon to do a little snap (tbh it's barely noticable but it's kinda annoying me), so checking with 0.1f instead.
     */
    if((Math::abs(m_MouseInput.x) < 0.1f) && (Math::abs(m_MouseInput.y) < 0.1f))
    {
      weapon_sway_component->weapon_idle_sway(delta);
    } else {
      weapon_sway_component->weapon_sway(delta, m_MouseVel);
    }
    
  }
  
  weapon_sway_component->reset_weapon_sway(delta);

  m_MouseInput.x = Math::lerp(m_MouseInput.x, Math::sign(m_MouseInput.x) * 0.1f, MOUSE_INPUT_RESET_MULTIPLIER * (float)delta);
  m_MouseInput.y = Math::lerp(m_MouseInput.y, Math::sign(m_MouseInput.y) * 0.1f, MOUSE_INPUT_RESET_MULTIPLIER * (float)delta);
}

WeaponManager::~WeaponManager()
{
}
