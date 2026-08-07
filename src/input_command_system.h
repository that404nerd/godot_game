#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

struct InputCommandData 
{
  bool WantsToSprint = false;
  bool WantsToJump = false;
  bool WantsToCrouch = false;

  bool WantsToShootWeapon = false;
  bool WantsToHoldShoot = false;
  bool WantsToReleaseShoot = false;

  bool WantsToReloadWeapon = false;
  bool WantsToSwitchWeapon = false;

  int WeaponListSize = 0;
  int WeaponIdx = 0;

  float MaxHoldTime = 0.0f;

  Vector2 MouseVel = Vector2(0.0f, 0.0f);
  Vector2 InputDir = Vector2(0.0f, 0.0f);
};

class InputCommandSystem : public Node
{
  GDCLASS(InputCommandSystem, Node);
public:

  virtual void _init() {};

  void set_wants_to_sprint(bool status) { m_InputCmdData.WantsToSprint = status; }
  void set_wants_to_jump(bool status) { m_InputCmdData.WantsToJump = status; }
  void set_wants_to_crouch(bool status) { m_InputCmdData.WantsToCrouch = status; }
  void set_wants_to_shoot_weapon(bool status) { m_InputCmdData.WantsToShootWeapon = status; }
  void set_wants_to_hold_shoot(bool status) { m_InputCmdData.WantsToHoldShoot = status; }
  void set_wants_to_release_shoot(bool status) { m_InputCmdData.WantsToReleaseShoot = status; }
  void set_wants_to_reload_weapon(bool status) { m_InputCmdData.WantsToReloadWeapon = status; }
  void set_wants_to_switch_weapon(bool status) { m_InputCmdData.WantsToSwitchWeapon = status; }

  void set_input_dir(Vector2 inputDir) { m_InputCmdData.InputDir = inputDir; }
  void set_mouse_vel(Vector2 mouseVel) { m_InputCmdData.MouseVel = mouseVel; }

  bool wants_to_sprint() { return m_InputCmdData.WantsToSprint; };
  bool wants_to_jump() { return m_InputCmdData.WantsToJump; };
  bool wants_to_crouch() { return m_InputCmdData.WantsToCrouch; };

  bool wants_to_hold_shoot() { return m_InputCmdData.WantsToHoldShoot; };
  bool wants_to_shoot_weapon() { return m_InputCmdData.WantsToShootWeapon; };
  bool wants_to_release_shoot() { return m_InputCmdData.WantsToReleaseShoot; };

  bool wants_to_reload_weapon() { return m_InputCmdData.WantsToReloadWeapon; };
  bool wants_to_switch_weapon() { return m_InputCmdData.WantsToSwitchWeapon; };

  void set_weapon_idx(int val) { m_InputCmdData.WeaponIdx = val; }
  int get_weapon_idx() { return m_InputCmdData.WeaponIdx; }

  void set_weapon_list_size(int val) { m_InputCmdData.WeaponListSize = val; }
  int get_weapon_list_size() { return m_InputCmdData.WeaponListSize; }

  Vector2 get_mouse_vel() { return m_InputCmdData.MouseVel; };
  Vector2 get_input_dir() { return m_InputCmdData.InputDir; };

  void set_max_hold_time(float val) { m_InputCmdData.MaxHoldTime = val; }
  float get_max_hold_time() { return m_InputCmdData.MaxHoldTime; }

  virtual void _update(double delta) {};

public:
  static void _bind_methods() {};
private:
  InputCommandData m_InputCmdData;
};