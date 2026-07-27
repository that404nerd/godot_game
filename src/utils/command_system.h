#pragma once

#include <godot_cpp/godot.hpp>

using namespace godot;

struct CommandData 
{
  bool WantsToMove = false;
  bool WantsToJump = false;
  bool WantsToCrouch = false;
};

class CommandSystem 
{
public:
  bool wants_to_move() { return m_CmdData.WantsToMove; };
  bool wants_to_jump() { return m_CmdData.WantsToJump; };
  bool wants_to_crouch() { return m_CmdData.WantsToCrouch; };

protected:
  CommandData& get_command_data() { return m_CmdData; }

private:
  CommandData m_CmdData;
};