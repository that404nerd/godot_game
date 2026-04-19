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

private:
  struct WeaponStateMachineData 
  {
    WeaponManager* weaponManager;
    WeaponComponent* weaponComponent;
    CharacterComponent* characterComponent;
  };

public:
  void _init_data(const WeaponStateMachineData& weaponStateMachineData);

  Array get_weapon_anim_groups() const { return m_WeaponAnimGroups; }
  CharacterComponent* get_character_component() const { return m_CharacterComponent; }
  WeaponComponent* get_weapon_component() const { return m_WeaponComponent; }

protected:
  static void _bind_methods();

private:
  CharacterComponent* m_CharacterComponent { nullptr };
  WeaponComponent* m_WeaponComponent { nullptr };

  Array m_WeaponAnimGroups;
};