#include "ai_character_component.h"
#include "../../managers/ai_manager.h"

void AICharacterComponent::_ready()
{
  CharacterComponent::_ready();

  ai_state_machine->set_initial_state(initial_state);
  ai_state_machine->initialize(this);
  ai_state_machine->set_active(true);

  if(ai_manager)
    ai_manager->_init();
}

void AICharacterComponent::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(AICharacterComponent, AIManager, ai_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AICharacterComponent, AIBehaviourProps, ai_behaviour_props, Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AICharacterComponent, LimboHSM, ai_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AICharacterComponent, LimboState, initial_state, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

}

void AICharacterComponent::_unhandled_input(const Ref<InputEvent>& event)
{
}

void AICharacterComponent::_process(double delta)
{
  CharacterComponent::_process(delta);

  if(ai_manager)
    ai_manager->_update(delta);
}

void AICharacterComponent::_physics_process(double delta)
{
  CharacterComponent::_physics_process(delta);

  if(ai_manager)
    ai_manager->_physics_update(delta);

}