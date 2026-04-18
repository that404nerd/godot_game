#pragma once

#include "state_machine.h"
#include "weapon_states.h"

#include <memory>

class WeaponManager;
class WeaponIdleState;
class WeaponEquipState;
class WeaponShootState;
class WeaponReloadState;
 
enum class WeaponStates : uint8_t {
  IDLE, EQUIP, SHOOT, RELOAD, UNEQUIP, WEAPON_SWITCH
};

class WeaponStateMachine : public StateMachine {

  GDCLASS(WeaponStateMachine, StateMachine);

public:
  void _init_data(WeaponManager* weaponManager, CharacterComponent* characterComponent);

  const Array get_weapon_anim_groups() const { return m_WeaponAnimGroups; }

  const WeaponManager* get_weapon_manager_inst() const { return m_WeaponManagerInst; }
  CharacterComponent* get_character_component() { return m_CharacterComponent; }

protected:
  static void _bind_methods();

private:
  WeaponManager* m_WeaponManagerInst { nullptr };
  CharacterComponent* m_CharacterComponent { nullptr };

  Array m_WeaponAnimGroups;
};