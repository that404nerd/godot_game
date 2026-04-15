#pragma once

#include <godot_cpp/godot.hpp>

#include "state_machine.h"

using namespace godot;

class WeaponStateManager : public StateMachine {
public:
  struct WeaponStateData 
  {
    WeaponComponent weaponComponent;
  };

};