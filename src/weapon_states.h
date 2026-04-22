#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include "state.h"
#include "weapon_manager.h"

using namespace godot;

class WeaponStateMachine;

class WeaponIdleState : public State {
public:
  WeaponIdleState(WeaponStateMachine* weaponStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  WeaponComponent* m_WeaponComponent { nullptr };
};

class WeaponEquipState : public State {
public:
  WeaponEquipState(WeaponStateMachine* weaponStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
};

class WeaponShootState : public State {
public:
  WeaponShootState(WeaponStateMachine* weaponStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };

private:
  Weapon::WeaponType m_WeaponType;
  float m_ShootTimeBeforeIdle;
  bool m_WantsToShoot, m_IsKeyHeld;
};

class WeaponReloadState : public State {
public:
  WeaponReloadState(WeaponStateMachine* weaponStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
};

class WeaponUnequipState : public State {
public:
  WeaponUnequipState(WeaponStateMachine* weaponStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

  void _unequip_weapon();

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponComponent* m_WeaponComponent { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
};

class WeaponSwitchState : public State {
public:
  WeaponSwitchState(WeaponStateMachine* weaponStateMachine);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

  void _weapon_switch();

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  WeaponComponent* m_WeaponComponent { nullptr };

  Ref<Weapon> m_CurrentWeapon { nullptr };
};