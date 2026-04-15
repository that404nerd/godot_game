#pragma once

class Player;

class GlobalStateHandler {
public:
  GlobalStateHandler(Player* player);

  void _enter();
  void _physics_update(double delta);

  ~GlobalStateHandler();

private:
  Player* m_PlayerInst { nullptr };
};
