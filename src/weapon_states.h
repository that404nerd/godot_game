#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include "components/ammo_component.h"
#include "state.h"

using namespace godot;

class WeaponManager;
class WeaponStateMachine;

struct WeaponStateContext
{
  bool WantsToShoot { false }, IsKeyHeld { false };
  float ShootTimeBeforeIdle { 1.0f };
  Weapon::WeaponType CurrentWeaponType { Weapon::WeaponType::NONE };
};

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