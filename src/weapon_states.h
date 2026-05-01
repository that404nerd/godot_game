#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include "components/ammo_component.h"
#include "state.h"
#include "weapon_manager.h"

using namespace godot;

class WeaponStateMachine;

struct WeaponStateData
{
  WeaponManager* weaponManager;
  WeaponStateMachine* weaponStateMachine;
};

class WeaponIdleState : public State {
public:
  WeaponIdleState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };

  WeaponComponent* m_WeaponComponent { nullptr };
};

class WeaponEquipState : public State {
public:
  WeaponEquipState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  AmmoComponent* m_AmmoComponent;
};

class WeaponShootState : public State {
public:
  WeaponShootState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };

  AmmoComponent* m_AmmoComponent { nullptr };

private:
  Weapon::WeaponType m_WeaponType;
  float m_ShootTimeBeforeIdle;
  bool m_WantsToShoot, m_IsKeyHeld;
};

class WeaponReloadState : public State {
public:
  WeaponReloadState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  AmmoComponent* m_AmmoComponent { nullptr };
};

class WeaponUnequipState : public State {
public:
  WeaponUnequipState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

  void _unequip_weapon();

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponComponent* m_WeaponComponent { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  WeaponManager* m_WeaponManager { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
};

class WeaponSwitchState : public State {
public:
  WeaponSwitchState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

  void _weapon_switch();

private:
  AnimationPlayer* m_WeaponAnimPlayer { nullptr };
  WeaponManager* m_WeaponManager { nullptr };
  WeaponComponent* m_WeaponComponent { nullptr };

  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
};