#pragma once

#include <functional>
#include <unordered_map>

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/vector2.hpp>

class CharacterComponent;

using namespace godot;

/*
  Rewrite the whole damn thing because this is terrible!!!
*/

enum class InputCommands
{
  SHOOT, HOLD_SHOOT, RELEASE_SHOOT, RELOAD, SWITCH_WEAPON
};

struct InputCommandData 
{
  bool WantsToShootWeapon = false;
  bool WantsToHoldShoot = false;
  bool WantsToReleaseShoot = false;

  bool WantsToReloadWeapon = false;
  bool WantsToSwitchWeapon = false;

  int WeaponListSize = 0;
  int WeaponIdx = 0;
  float MaxHoldTime = 0.0f;
  Vector2 MouseVel = Vector2(0.0f, 0.0f);
};

struct MoveCommand
{
  Vector3 CharacterWishDir = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 CharacterVelocity = Vector3(0.0f, 0.0f, 0.0f);
  Vector2 InputDir = Vector2(0.0f, 0.0f);

  bool IsOnFloor = false;

  bool WantsToMove = false;
  bool WantsToSprint = false;
  bool WantsToIdle = false;
  bool WantsToJump = false;
  bool WantsToCrouch = false;
};

class InputCommandSystem : public Node
{
  GDCLASS(InputCommandSystem, Node);
public:
  void _init();

  virtual void _update(double delta) {};

  void command(InputCommands inputCommand, bool clearPrevCommands=true);

  virtual void build_move_command(CharacterComponent* characterComp) {};
  void set_mouse_vel(Vector2 mouseVel) { m_InputCmdData.MouseVel = mouseVel; }
  void set_max_hold_time(float val) { m_InputCmdData.MaxHoldTime = val; }
  float get_max_hold_time() { return m_InputCmdData.MaxHoldTime; }

  MoveCommand& get_move_command() { return m_MoveCmd; }

  Vector2 get_mouse_vel() { return m_InputCmdData.MouseVel; };
  Vector2 get_input_dir() { return m_MoveCmd.InputDir; };

  bool wants_to_hold_shoot() { return m_InputCmdData.WantsToHoldShoot; };
  bool wants_to_shoot_weapon() { return m_InputCmdData.WantsToShootWeapon; };
  bool wants_to_release_shoot() { return m_InputCmdData.WantsToReleaseShoot; };

  bool wants_to_reload_weapon() { return m_InputCmdData.WantsToReloadWeapon; };
  bool wants_to_switch_weapon() { return m_InputCmdData.WantsToSwitchWeapon; };

  void set_weapon_idx(int val) { m_InputCmdData.WeaponIdx = val; }
  int get_weapon_idx() { return m_InputCmdData.WeaponIdx; }

  void set_weapon_list_size(int val) { m_InputCmdData.WeaponListSize = val; }
  int get_weapon_res_list_size() { return m_InputCmdData.WeaponListSize; }

  void set_wants_to_shoot_weapon(bool status) { m_InputCmdData.WantsToShootWeapon = status; }
  void set_wants_to_hold_shoot(bool status) { m_InputCmdData.WantsToHoldShoot = status; }
  void set_wants_to_release_shoot(bool status) { m_InputCmdData.WantsToReleaseShoot = status; }
  void set_wants_to_reload_weapon(bool status) { m_InputCmdData.WantsToReloadWeapon = status; }
  void set_wants_to_switch_weapon(bool status) { m_InputCmdData.WantsToSwitchWeapon = status; }

protected:
  static void _bind_methods();

protected:
  MoveCommand m_MoveCmd {};
private:
  std::unordered_map<InputCommands, std::function<void(bool)>> m_InputCommandList {};
  InputCommandData m_InputCmdData;
};