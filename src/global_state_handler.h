#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>

#include "game_manager.h"

using namespace godot;

class Player;

class GlobalStateHandler : public Node {
  GDCLASS(GlobalStateHandler, Node);

protected:
  static void _bind_methods();

public:
  GlobalStateHandler();

  void _ready() override;

  void _physics_process(double delta) override;

  ~GlobalStateHandler();

private:
  Player* m_PlayerInst = nullptr;
};
