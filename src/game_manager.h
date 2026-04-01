#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/object.hpp>

using namespace godot;

class Player;
class PlayerStateMachine;
class WeaponManager;

class GameManager : public Object { 

  GDCLASS(GameManager, Object);
  static GameManager* s_Singleton;

public:
  GameManager();
  static GameManager* get_singleton();
  
  void set_player_inst(Player* player_inst);
  Player* get_player_inst();

  void set_state_machine_inst(PlayerStateMachine* stateMachine);
  PlayerStateMachine* get_player_state_machine();

  void set_weapon_manager_inst(WeaponManager* weaponManagerInst);
  WeaponManager* get_weapon_manager_inst();


  ~GameManager();
protected:
  static void _bind_methods();

private:
  Player* m_PlayerInst = nullptr;
  PlayerStateMachine* m_PlayerStateMachineInst = nullptr;
  WeaponManager* m_WeaponManager = nullptr;
};
