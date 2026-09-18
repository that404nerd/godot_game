#include "input_command_system.h"

void InputCommandSystem::_init()
{
  m_MovementInputCommandList[MovementInputCommands::IDLE] = [this](bool val) { set_wants_to_idle(val); };
  m_MovementInputCommandList[MovementInputCommands::WALK] = [this](bool val) { set_wants_to_walk(val); };
  m_MovementInputCommandList[MovementInputCommands::SPRINT] = [this](bool val) { set_wants_to_sprint(val); };
  m_MovementInputCommandList[MovementInputCommands::JUMP] = [this](bool val) { set_wants_to_jump(val); };
  m_MovementInputCommandList[MovementInputCommands::CROUCH] = [this](bool val) { set_wants_to_crouch(val); };

  m_WeaponInputCommandList[WeaponInputCommands::SHOOT] = [this](bool val) { set_wants_to_shoot_weapon(val); };
  m_WeaponInputCommandList[WeaponInputCommands::HOLD_SHOOT] = [this](bool val) { set_wants_to_hold_shoot(val); };
  m_WeaponInputCommandList[WeaponInputCommands::RELEASE_SHOOT] = [this](bool val) { set_wants_to_release_shoot(val); };
  m_WeaponInputCommandList[WeaponInputCommands::RELOAD] = [this](bool val) { set_wants_to_reload_weapon(val); };
  m_WeaponInputCommandList[WeaponInputCommands::SWITCH_WEAPON] = [this](bool val) { set_wants_to_switch_weapon(val); };
}

void InputCommandSystem::_bind_methods()
{
}

void InputCommandSystem::command(MovementInputCommands inputCommand)
{
  for(auto& commandList : m_MovementInputCommandList)
  {
    auto& func = commandList.second;

    // Here commandList.first refers to the key from the HashMap
    if(commandList.first == inputCommand)
      func(true);
    else
      func(false);
  }
}

void InputCommandSystem::command(WeaponInputCommands inputCommand)
{
  for(auto& commandList : m_WeaponInputCommandList)
  {
    auto& func = commandList.second;

    // Here commandList.first refers to the key from the HashMap
    if(commandList.first == inputCommand)
      func(true);
    else
      func(false);
  }
}