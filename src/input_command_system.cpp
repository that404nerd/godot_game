#include "input_command_system.h"

void InputCommandSystem::_init()
{
}

void InputCommandSystem::_bind_methods()
{
}

bool InputCommandSystem::has(InputCommands inputCommand)
{
  for(auto& command : m_InputCommands)
  {
    if(inputCommand == command)
    {
      auto cmd = magic_enum::enum_cast<InputCommands>(static_cast<int>(inputCommand));
      auto name = magic_enum::enum_name(cmd.value());
      print_line(std::string(name).c_str());
      return true;
    }
  }

  return false;
}

void InputCommandSystem::command(InputCommands inputCommand)
{
  m_InputCommands.push_back(inputCommand);
}

void InputCommandSystem::clear()
{
  m_InputCommands.clear();
}