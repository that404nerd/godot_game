#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include "../globals.h"
#include "character_component.h"
#include "weapon_component.h"

using namespace godot;

class Weapon;

class WeaponBobComponent
{
public:
  void _init_data(CharacterComponent* characterComponent, WeaponComponent* weaponComponent);

  void _update_bob_data(Ref<Weapon> currentWeapon);

  void weapon_bob(double delta);

  Vector3 get_weapon_bob_offset() const { return m_BobOffset; }

private:
  Vector3 m_BobOffset {};
  Vector3 m_CharacterVel {};

  float m_WeaponBobTime { 0.0f };
  float m_WeaponBobFreq { 0.0f };
  float m_WeaponBobAmp { 0.0f };
  float m_WeaponBobSmoothVal { 0.0f };

  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
};

class WeaponSwayComponent
{
public:
  void _init_data(CharacterComponent* characterComponent, WeaponComponent* weaponComponent);
  void _update_sway_data(Ref<Weapon> currentWeapon);

  void weapon_idle_sway(double delta);
  void weapon_sway(double delta, Vector3& sway_vel);

  Vector3 get_idle_offset() const { return m_IdleSwayOffset; }
  Vector3 get_sway_offset() const { return m_SwayOffset; }

private:
  Vector3 m_IdleSwayOffset {};
  Vector3 m_SwayOffset {};

  float m_IdleWeaponBobTime { 0.0f };

  float m_IdleWeaponBobFreq { 0.0f };
  float m_IdleWeaponBobAmp { 0.0f };
  float m_IdleWeaponBobSmoothVal { 0.0f };

  float m_WeaponSpringAngFreq { 0.0f };
  float m_WeaponSpringDampingRatio { 0.0f };

  Utils::tDampedSpringMotionParams m_SwayParams;

  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
};

class WeaponEffects
{
public:
  void _init_data(Node3D* holdPointNode,
                  CharacterComponent* characterComponent,
                  WeaponComponent* weaponComponent);

  void _update_data(Ref<Weapon> currentWeapon);
  void _update(double delta, Vector3& sway_vel);

private:
  Node3D* m_HoldPointNode { nullptr };
  Vector3 m_BasePos {};

  WeaponBobComponent m_WeaponBobComponent;
  WeaponSwayComponent m_WeaponSwayComponent;
};;