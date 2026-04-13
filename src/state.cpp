#include "state.h"

State::State() {}

void State::_bind_methods() 
{
  ADD_SIGNAL(MethodInfo("state_changed", PropertyInfo(Variant::STRING, "stateName")));
}
