#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/math.hpp>

#include "../globals.h"
#include "character_component.h"
#include "godot_cpp/classes/character_body3d.hpp"
#include "weapon_component.h"

using namespace godot;

struct WeaponEffectsData
{
  CharacterComponent* characterComponent;
  WeaponComponent* weaponComponent;
  Node3D* holdPoint;  
};

class WeaponBobComponent
{
public:

  void _init_data(const WeaponEffectsData& weaponEffectsData)
  {
    m_CharacterBody = weaponEffectsData.characterComponent->get_character_body();

    m_HoldPointNode = weaponEffectsData.holdPoint;

    // TODO: Do the checking and assignment only during weapon_change state
    m_CurrentWeapon = weaponEffectsData.weaponComponent->get_current_weapon_data();
  
    m_WeaponBobFreq = m_CurrentWeapon->get_weapon_bob_freq();
    m_WeaponBobAmp = m_CurrentWeapon->get_weapon_bob_amp();
    m_WeaponBobSmoothVal = m_CurrentWeapon->get_weapon_bob_smooth_val();
    m_IdleWeaponBobFreq = m_CurrentWeapon->get_idle_weapon_bob_freq();
    m_IdleWeaponBobAmp = m_CurrentWeapon->get_idle_weapon_bob_amp();
    m_IdleWeaponBobSmoothVal = m_CurrentWeapon->get_idle_weapon_bob_smooth_val();
  }

  void weapon_bob(double delta)
  {
    if(!m_CharacterBody || !m_HoldPointNode)
    {
      return;
    }

    bool onFloor = m_CharacterBody->is_on_floor(); // so that bobbing doesn't occur during airborne states

    float velocity = m_CharacterBody->get_velocity().length();

    m_WeaponBobTime += delta * velocity * onFloor;

    float x_bob = Math::cos(m_WeaponBobTime * m_WeaponBobFreq * 0.5f) * m_WeaponBobAmp;
    float y_bob = Math::sin(m_WeaponBobTime * m_WeaponBobFreq) * m_WeaponBobAmp;

    Vector3 currentPos = m_HoldPointNode->get_position();
    Vector3 newPos = Vector3(
      Utils::exp_decay(currentPos.x, x_bob, m_WeaponBobSmoothVal, (float)delta),
      Utils::exp_decay(currentPos.y, y_bob, m_WeaponBobSmoothVal, (float)delta), 
      0.0f
    );

    m_HoldPointNode->set_position(newPos);
  }

private:
  float m_WeaponBobFreq { 0.0f }, m_WeaponBobAmp { 0.0f }, m_WeaponBobSmoothVal { 0.0f };
  float m_IdleWeaponBobFreq { 0.0f }, m_IdleWeaponBobAmp { 0.0f }, m_IdleWeaponBobSmoothVal { 0.0f };
  float m_WeaponBobTime { 0.0f };

  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr }; // This is used to read the current weapon data set in the weapon component
  
private:
  Node3D* m_HoldPointNode { nullptr };
};


class WeaponSwayComponent
{
public:

  void _init_data(const WeaponEffectsData& weaponEffectsData)
  {
    m_CharacterBody = weaponEffectsData.characterComponent->get_character_body();
    m_HoldPointNode = weaponEffectsData.holdPoint;

    // TODO: Do the checking and assignment only during weapon_change state
    m_CurrentWeapon = weaponEffectsData.weaponComponent->get_current_weapon_data();
  
    m_WeaponSwayMult = m_CurrentWeapon->get_weapon_sway_mult();
    m_WeaponSwayResetValue = m_CurrentWeapon->get_weapon_sway_reset();
    m_IdleWeaponBobFreq = m_CurrentWeapon->get_idle_weapon_bob_freq();
    m_IdleWeaponBobAmp = m_CurrentWeapon->get_idle_weapon_bob_amp();
    m_IdleWeaponBobSmoothVal = m_CurrentWeapon->get_idle_weapon_bob_smooth_val();
  }

  void reset_weapon_sway(double delta)
  {
    m_HoldPointPos = m_HoldPointNode->get_position();
    m_HoldPointPos.x = Math::lerp(m_HoldPointPos.x, 0.0f, m_WeaponSwayResetValue * (float)delta);
    m_HoldPointPos.y = Math::lerp(m_HoldPointPos.y, 0.0f, m_WeaponSwayResetValue * (float)delta);

    m_HoldPointNode->set_position(m_HoldPointPos);
  }

  void weapon_idle_sway(double delta)
  {
    m_IdleWeaponBobTime += delta * 0.5f;

    float x_bob = Math::cos(m_IdleWeaponBobTime * m_IdleWeaponBobFreq * 0.5f) * m_IdleWeaponBobAmp;
    float y_bob = Math::sin(m_IdleWeaponBobTime * m_IdleWeaponBobFreq) * m_IdleWeaponBobAmp;

    Vector3 currentPos = m_HoldPointNode->get_position();
    Vector3 newPos = Vector3(
      Utils::exp_decay(currentPos.x, x_bob, m_IdleWeaponBobSmoothVal, (float)delta),
      Utils::exp_decay(currentPos.y, y_bob, m_IdleWeaponBobSmoothVal, (float)delta), 
      0.0f
    );

    m_HoldPointNode->set_position(newPos);
  }

  void weapon_sway(double delta, Vector2 sway_vector)
  {
    if(!m_CharacterBody || !m_HoldPointNode) return;

    m_HoldPointPos = m_HoldPointNode->get_position();
    m_HoldPointPos.x -= sway_vector.x * m_WeaponSwayMult * delta;
    m_HoldPointPos.y += sway_vector.y * m_WeaponSwayMult * delta;

    m_HoldPointPos = m_HoldPointPos.clamp(Vector3(-0.008f, -0.001f, 0.0f), Vector3(0.008f, 0.001f, 0.0f));

    m_HoldPointNode->set_position(m_HoldPointPos);
  }

private:
  float m_WeaponSwayMult { 0.0f }, m_WeaponSwayResetValue { 0.0f };
  float m_IdleWeaponBobTime { 0.0f }, m_IdleWeaponBobFreq { 0.0f }, m_IdleWeaponBobAmp { 0.0f }, m_IdleWeaponBobSmoothVal;

  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr };
  Vector3 m_HoldPointPos { Vector3(0.0f, 0.0f, 0.0f ) };
  
private:
  Node3D* m_HoldPointNode { nullptr };
};
