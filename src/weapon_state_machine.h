#pragma once

#include "components/weapon_component.h"
#include "godot_cpp/classes/animation_player.hpp"
#include "state_machine.h"
#include "weapon_states.h"

#include "magic_enum/magic_enum.hpp"
#include <memory>

class WeaponManager;
class WeaponIdleState;
class WeaponEquipState;
class WeaponShootState;
class WeaponReloadState;
class WeaponUnequipState;
class WeaponSwitchState;
 
enum class WeaponStates : int8_t {
  NONE = -1, IDLE, EQUIP, SHOOT, RELOAD, UNEQUIP, WEAPON_SWITCH
};

class WeaponStateMachine : public StateMachine {

  GDCLASS(WeaponStateMachine, StateMachine);

public:
  void _init_data() override;
  void _handle_state_machine_input(const Ref<InputEvent>& event) override;

  Array get_weapon_anim_groups() const { return m_WeaponAnimGroups; }
  AnimationPlayer* get_current_weapon_anim_player();

  StringName get_current_state_name()
  {
    auto none_name = magic_enum::enum_name(WeaponStates::NONE);
    StringName noneString = std::string(none_name).c_str();
    if(m_CurrentState == nullptr)
    {
      print_error("Current state is null!");
      return noneString;
    }

    auto state = magic_enum::enum_cast<WeaponStates>(m_CurrentState->get_current_state());
    auto stateName = magic_enum::enum_name(state.value());
    StringName finalStateName = std::string(stateName).c_str();

    return finalStateName;
  }

  void _on_animation_finished(const StringName& anim_name);

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);

  GD_DEFINE_PROPERTY(float, shoot_time_before_idle, 0.3f);

  int m_WeaponIndex { 0 };

  Array m_WeaponAnimGroups;
  AnimationPlayer* m_CurrentWeaponAnimPlayer { nullptr };
};