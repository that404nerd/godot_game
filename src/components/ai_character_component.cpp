#include "ai_character_component.h"
#include "../state_machines/ai_state_machine.h"

void AICharacterComponent::_ready()
{
  CharacterComponent::_ready();

  if(ai_state_machine)
    ai_state_machine->_init();
}

void AICharacterComponent::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(AICharacterComponent, ai_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
}

void AICharacterComponent::_unhandled_input(const Ref<InputEvent>& event)
{
}

void AICharacterComponent::_process(double delta)
{
  CharacterComponent::_process(delta);

  if(ai_state_machine)
    ai_state_machine->_update(delta);
}

void AICharacterComponent::_physics_process(double delta)
{
  CharacterComponent::_physics_process(delta);

  if(ai_state_machine)
    ai_state_machine->_physics_update(delta);
}