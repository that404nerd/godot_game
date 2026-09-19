#pragma once

#include <functional>

#include "magic_enum/magic_enum.hpp"

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <unordered_map>

using namespace godot;

enum class InputCommands
{
  IDLE, WALK, SPRINT, JUMP, CROUCH,
  SHOOT, HOLD_SHOOT, RELEASE_SHOOT, RELOAD, SWITCH_WEAPON
};

struct InputCommandData 
{
  int WeaponListSize = 0;
  int WeaponIdx = 0;

  float MaxHoldTime = 0.0f;

  Vector3 CharacterWishDir = Vector3(0.0f, 0.0f, 0.0f);
  Vector2 MouseVel = Vector2(0.0f, 0.0f);
  Vector2 InputDir = Vector2(0.0f, 0.0f);
};

class InputCommandSystem : public Node
{
  GDCLASS(InputCommandSystem, Node);
public:

  void _init();

  virtual void _update(double delta) {};

  void command(InputCommands inputCommand);
  bool has(InputCommands inputCommand);
  void clear();

  void set_wish_dir(Vector3 wishDir) { m_InputCmdData.CharacterWishDir = wishDir; }
  void set_input_dir(Vector2 inputDir) { m_InputCmdData.InputDir = inputDir; }
  void set_mouse_vel(Vector2 mouseVel) { m_InputCmdData.MouseVel = mouseVel; }

  void set_max_hold_time(float val) { m_InputCmdData.MaxHoldTime = val; }
  float get_max_hold_time() { return m_InputCmdData.MaxHoldTime; }

  Vector3 get_character_wish_dir() { return m_InputCmdData.CharacterWishDir; };
  Vector2 get_mouse_vel() { return m_InputCmdData.MouseVel; };
  Vector2 get_input_dir() { return m_InputCmdData.InputDir; };
  void set_next_weapon_idx(int val) { m_InputCmdData.WeaponIdx = val; }
  int get_weapon_idx() { return m_InputCmdData.WeaponIdx; }

  void set_weapon_list_size(int val) { m_InputCmdData.WeaponListSize = val; }
  int get_weapon_list_size() { return m_InputCmdData.WeaponListSize; }

protected:
  static void _bind_methods();

private:
  LocalVector<InputCommands> m_InputCommands;
  InputCommandData m_InputCmdData;
};