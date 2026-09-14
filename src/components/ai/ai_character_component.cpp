#include "ai_character_component.h"
#include "../../managers/ai_manager.h"
#include "../../state_machines/ai_state_machine.h"

void AICharacterComponent::_ready()
{
  CharacterComponent::_ready();

  if(ai_state_machine)
    ai_state_machine->_init();

  if(ai_manager)
    ai_manager->_init();

}

void AICharacterComponent::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(AICharacterComponent, AIManager, ai_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AICharacterComponent, AIStateMachine, ai_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(AICharacterComponent, AIBehaviourProps, ai_behaviour_props, Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE);

}

void AICharacterComponent::_unhandled_input(const Ref<InputEvent>& event)
{
}

void AICharacterComponent::_process(double delta)
{
  CharacterComponent::_process(delta);

  if(ai_state_machine)
    ai_state_machine->_update(delta);

  if(ai_manager)
    ai_manager->_update(delta);
}

void AICharacterComponent::_physics_process(double delta)
{
  CharacterComponent::_physics_process(delta);

  if(ai_state_machine)
    ai_state_machine->_physics_update(delta);

  if(ai_manager)
    ai_manager->_physics_update(delta);

}