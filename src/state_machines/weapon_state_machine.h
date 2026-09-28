#pragma once

#include <memory>

#include <godot_cpp/classes/animation_player.hpp>
#include "magic_enum/magic_enum.hpp"

#include "../components/play_anim_component.h"
#include "../input_command_system.h"
#include "../components/weapon_component.h"
#include "./state_machine.h"

class WeaponManager;

enum WeaponStates {
  NONE = -1, IDLE, EQUIP, SHOOT, RELOAD, UNEQUIP, WEAPON_SWITCH
};

class WeaponStateMachine;

struct WeaponStateData
{
  WeaponManager* weaponManager;
  WeaponStateMachine* weaponStateMachine;
};

class WeaponStateMachine : public StateMachine {

  GDCLASS(WeaponStateMachine, StateMachine);

public:
  void _init_data() override;
  void _handle_state_machine_input(const Ref<InputEvent>& event) override;

  StringName get_current_state_name();

  void _on_animation_finished(const StringName& anim_name);

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(WeaponManager*, weapon_manager, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponStates, default_weapon_state, WeaponStates::EQUIP);

  PlayAnimComponent* m_PlayAnimComp { nullptr };
  InputCommandSystem* m_InputCmdSystem { nullptr };
  WeaponStateData m_WeaponStateData;
  Ref<Weapon> m_CurrentWeapon { nullptr };
};
VARIANT_ENUM_CAST(WeaponStates);