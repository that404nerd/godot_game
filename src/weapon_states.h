#pragma once

#include <godot_cpp/godot.hpp>

#include "state.h"
#include "weapon_manager.h"
#include "weapon_state_machine.h"

using namespace godot;

class WeaponIdleState : public State {
public:
  WeaponIdleState(WeaponManager* weaponManager);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
};

class WeaponEquipState : public State {
public:
  WeaponEquipState(WeaponManager* weaponManager);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  WeaponManager* m_WeaponManager { nullptr };
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
};

class WeaponShootState : public State {
public:
  WeaponShootState(WeaponManager* weaponManager);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  WeaponManager* m_WeaponManager { nullptr };
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };

private:
  bool m_DidShoot, m_WantsToShoot;
  float m_TimeBetweenShots;
};
