#pragma once

#include "components/weapon_component.h"
#include "state_machine.h"
#include "weapon_states.h"

#include <memory>

class WeaponManager;
class WeaponIdleState;
class WeaponEquipState;
class WeaponShootState;
class WeaponReloadState;
class WeaponUnequipState;
 
enum class WeaponStates : uint8_t {
  IDLE, EQUIP, SHOOT, RELOAD, UNEQUIP, WEAPON_SWITCH
};

class WeaponStateMachine : public StateMachine {

  GDCLASS(WeaponStateMachine, StateMachine);

public:
  void _init_data() override;

  Array get_weapon_anim_groups() const { return m_WeaponAnimGroups; }

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);

  Array m_WeaponAnimGroups;

};