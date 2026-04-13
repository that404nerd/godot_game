#pragma once

#include <godot_cpp/godot.hpp>

#include "../globals.h"

using namespace godot;

class AmmoComponent {

public:

  void consume_ammo(int ammo)
  {

  }

private:
  int total_ammo { 0 };
  int current_ammo_total { 0 };
};
