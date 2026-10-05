#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>

class CharacterComponent;

using namespace godot;

struct InputCommandData 
{
   Vector2 MouseVel = Vector2(0.0f, 0.0f);
};

struct MoveCommand
{
  Vector3 CharacterWishDir = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 CharacterVelocity = Vector3(0.0f, 0.0f, 0.0f);
  Vector2 InputDir = Vector2(0.0f, 0.0f);

  bool IsOnFloor = false;

  bool WantsToSprint = false;
  bool WantsToWalk = false;
  bool WantsToIdle = false;
  bool WantsToJump = false;
  bool WantsToCrouch = false;
};

struct WeaponCommand
{
  int WeaponListSize = 0;
  int WeaponIdx = 0;
  float MaxHoldTime = 0.0f;

  bool WantsToPressTrigger = false;
  bool WantsToHoldTrigger = false;
  bool WantsToReleaseShoot = false;
  bool WantsToReloadWeapon = false;
  bool WantsToSwitchWeapon = false;
};

class InputCommandSystem : public Node
{
  GDCLASS(InputCommandSystem, Node);
public:
  void _init() {};

  virtual void _update(double delta);

  virtual void build_move_command(CharacterComponent* characterComp) {};
  void set_mouse_vel(Vector2 mouseVel) { m_InputCmdData.MouseVel = mouseVel; }
  void set_max_hold_time(float val) { m_WeaponCmd.MaxHoldTime = val; }
  float get_max_hold_time() { return m_WeaponCmd.MaxHoldTime; }

  MoveCommand& get_move_command() { return m_MoveCmd; }
  WeaponCommand& get_weapon_command() { return m_WeaponCmd; }

  Vector2 get_mouse_vel() { return m_InputCmdData.MouseVel; };
  Vector2 get_input_dir() { return m_MoveCmd.InputDir; };

  void set_weapon_idx(int val) { m_WeaponCmd.WeaponIdx = val; }
  int get_weapon_idx() { return m_WeaponCmd.WeaponIdx; }

  void set_weapon_list_size(int val) { m_WeaponCmd.WeaponListSize = val; }
  int get_weapon_res_list_size() { return m_WeaponCmd.WeaponListSize; }

protected:
  static void _bind_methods();

protected:
  MoveCommand m_MoveCmd {};
  WeaponCommand m_WeaponCmd {};
private:
  InputCommandData m_InputCmdData;
};