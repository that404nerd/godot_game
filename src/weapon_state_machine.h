#pragma once

#include <memory>

#include "magic_enum/magic_enum.hpp"

#include "components/weapon_component.h"
#include "state_machine.h"

class WeaponManager;

enum class WeaponStates {
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

  WeaponStateData m_WeaponStateData;
  Ref<Weapon> m_CurrentWeapon { nullptr };
};
