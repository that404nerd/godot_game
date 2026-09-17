#include "input_command_system.h"

void InputCommandSystem::_init()
{
  m_InputCommandList[InputCommands::IDLE] = [this](bool val) { set_wants_to_idle(val); };
  m_InputCommandList[InputCommands::WALK] = [this](bool val) { set_wants_to_walk(val); };
  m_InputCommandList[InputCommands::SPRINT] = [this](bool val) { set_wants_to_sprint(val); };
  m_InputCommandList[InputCommands::JUMP] = [this](bool val) { set_wants_to_jump(val); };
  m_InputCommandList[InputCommands::CROUCH] = [this](bool val) { set_wants_to_crouch(val); };


  m_InputCommandList[InputCommands::SHOOT] = [this](bool val) { set_wants_to_shoot_weapon(val); };
  m_InputCommandList[InputCommands::HOLD_SHOOT] = [this](bool val) { set_wants_to_hold_shoot(val); };
  m_InputCommandList[InputCommands::RELEASE_SHOOT] = [this](bool val) { set_wants_to_release_shoot(val); };
  m_InputCommandList[InputCommands::RELOAD] = [this](bool val) { set_wants_to_reload_weapon(val); };
  m_InputCommandList[InputCommands::SWITCH_WEAPON] = [this](bool val) { set_wants_to_switch_weapon(val); };
}

void InputCommandSystem::_bind_methods()
{
}

void InputCommandSystem::command(InputCommands inputCommand, bool clearPrevCommands)
{
  if(clearPrevCommands)
  {
    for(auto& commandList : m_InputCommandList)
    {
      auto& func = commandList.second;

      // Here commandList.first refers to the key from the HashMap
      if(commandList.first == inputCommand)
        func(true);
      else
        func(false);
    }
  }
}