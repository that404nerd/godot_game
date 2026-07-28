#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

struct CommandData 
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
  Vector2 InputDir = Vector2(0.0f, 0.0f);
};

class InputCommandSystem : public Node
{
  GDCLASS(InputCommandSystem, Node);
public:
  bool wants_to_sprint() { return m_CmdData.WantsToSprint; };
  bool wants_to_jump() { return m_CmdData.WantsToJump; };
  bool wants_to_crouch() { return m_CmdData.WantsToCrouch; };

  bool wants_to_hold_shoot() { return m_CmdData.WantsToHoldShoot; };
  bool wants_to_shoot_weapon() { return m_CmdData.WantsToShootWeapon; };
  bool wants_to_release_shoot() { return m_CmdData.WantsToReleaseShoot; };

  bool wants_to_reload_weapon() { return m_CmdData.WantsToReloadWeapon; };
  bool wants_to_switch_weapon() { return m_CmdData.WantsToSwitchWeapon; };

  void set_weapon_idx(int val) { m_CmdData.WeaponIdx = val; }
  int get_weapon_idx() { return m_CmdData.WeaponIdx; }

  void set_weapon_list_size(int val) { m_CmdData.WeaponListSize = val; }
  int get_weapon_list_size() { return m_CmdData.WeaponListSize; }

  Vector2 get_input_dir() { return m_CmdData.InputDir; };

protected:
  static void _bind_methods() {};
  CommandData& get_command_data() { return m_CmdData; }

private:
  CommandData m_CmdData;
};