#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include "../components/ammo_component.h"
#include "../weapon_state_machine.h"

#include "state.h"
#include "../weapon_state_machine.h"

class WeaponManager;

using namespace godot;

struct WeaponStateContext
{
  Weapon::WeaponType CurrentWeaponType { Weapon::WeaponType::NONE };
  float ShootTimeBeforeIdle { 1.0f };
  bool IsReloading { false }, IsReloadStarted { false };
  bool IsKeyPressed { false }, IsKeyHeld { false };
  bool IsUnequipped { false };
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
  WeaponStateContext& m_WeaponStateContext;
};

class WeaponEquipState : public State {
public:
  WeaponEquipState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
};

class WeaponShootState : public State {
public:
  WeaponShootState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  Ref<Weapon> m_CurrentWeapon { nullptr };

private:
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  const WeaponStateContext& m_WeaponStateContext;
};

class WeaponReloadState : public State {
public:
  WeaponReloadState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  const WeaponStateContext& m_WeaponStateContext;
  Ref<Weapon> m_CurrentWeapon { nullptr };
};

class WeaponUnequipState : public State {
public:
  WeaponUnequipState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  WeaponManager* m_WeaponManager { nullptr };
  const WeaponStateContext& m_WeaponStateContext;
};

class WeaponSwitchState : public State {
public:
  WeaponSwitchState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

private:
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
};
