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


class BaseWeaponState : public State {
public:
  BaseWeaponState(WeaponStates movementState, const WeaponStateData& weaponStateData);

protected:
  WeaponManager* m_WeaponManager { nullptr };
  WeaponStateMachine* m_WeaponStateMachine { nullptr };
  const WeaponStateContext& m_WeaponStateContext;
};

///////////////////////////// Weapon States Declaration //////////////////////////////////
class WeaponIdleState : public BaseWeaponState {
public:
  WeaponIdleState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;
};

class WeaponEquipState : public BaseWeaponState {
public:
  WeaponEquipState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;
};

class WeaponShootState : public BaseWeaponState {
public:
  WeaponShootState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

};

class WeaponReloadState : public BaseWeaponState {
public:
  WeaponReloadState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

};

class WeaponUnequipState : public BaseWeaponState {
public:
  WeaponUnequipState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;

};

class WeaponSwitchState : public BaseWeaponState {
public:
  WeaponSwitchState(const WeaponStateData& weaponStateData);

  void _enter() override;
  void _handle_input(const Ref<InputEvent>& event) override;
  void _update(double delta) override;
  void _exit() override;
};
