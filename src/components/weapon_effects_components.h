#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

#include "../globals.h"
#include "character_component.h"
#include "godot_cpp/core/math.hpp"
#include "weapon_component.h"

using namespace godot;

class WeaponBobComponent : public Node
{
  GDCLASS(WeaponBobComponent, Node)

protected:
  static void _bind_methods()
  {
    GD_BIND_CUSTOM_PROPERTY(WeaponBobComponent, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponBobComponent, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponBobComponent, hold_point_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  }

public:

  void _ready() override
  {
    if(character_component != nullptr)
    {
      m_CharacterBody = character_component->get_character_body();
    }

    // TODO: Do the checking and assignment only during weapon_change state
    if(weapon_component)
      m_CurrentWeapon = weapon_component->get_current_weapon_data();
  
    m_WeaponBobFreq = m_CurrentWeapon->get_weapon_bob_freq();
    m_WeaponBobAmp = m_CurrentWeapon->get_weapon_bob_amp();
    m_WeaponBobSmoothVal = m_CurrentWeapon->get_weapon_bob_smooth_val();
    m_IdleWeaponBobFreq = m_CurrentWeapon->get_idle_weapon_bob_freq();
    m_IdleWeaponBobAmp = m_CurrentWeapon->get_idle_weapon_bob_amp();
    m_IdleWeaponBobSmoothVal = m_CurrentWeapon->get_idle_weapon_bob_smooth_val();

  }

  void weapon_bob(double delta)
  {
    if(!m_CharacterBody || !hold_point_node) return;

    bool onFloor = m_CharacterBody->is_on_floor(); // so that bobbing doesn't occur during airborne states

    float velocity = m_CharacterBody->get_velocity().length();

    m_WeaponBobTime += delta * velocity * onFloor;

    float x_bob = Math::cos(m_WeaponBobTime * m_WeaponBobFreq * 0.5f) * m_WeaponBobAmp;
    float y_bob = Math::sin(m_WeaponBobTime * m_WeaponBobFreq) * m_WeaponBobAmp;

    Vector3 currentPos = hold_point_node->get_position();
    Vector3 newPos = Vector3(
      Utils::exp_decay(currentPos.x, x_bob, m_WeaponBobSmoothVal, (float)delta),
      Utils::exp_decay(currentPos.y, y_bob, m_WeaponBobSmoothVal, (float)delta), 
      0.0f
    );

    hold_point_node->set_position(newPos);
  }

  void _process(double delta) override
  {
  }

private:
  float m_WeaponBobFreq { 0.0f }, m_WeaponBobAmp { 0.0f }, m_WeaponBobSmoothVal { 0.0f };
  float m_IdleWeaponBobFreq { 0.0f }, m_IdleWeaponBobAmp { 0.0f }, m_IdleWeaponBobSmoothVal { 0.0f };
  float m_WeaponBobTime { 0.0f };

  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr }; // This is used to read the current weapon data set in the weapon component
  
private:
  GD_DEFINE_PROPERTY(Node3D*, hold_point_node, nullptr);
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
};


class WeaponSwayComponent : public Node
{
  GDCLASS(WeaponSwayComponent, Node)

protected:
  static void _bind_methods()
  {
    GD_BIND_CUSTOM_PROPERTY(WeaponSwayComponent, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponSwayComponent, weapon_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
    GD_BIND_CUSTOM_PROPERTY(WeaponSwayComponent, hold_point_node, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  }

public:

  void _ready() override
  {
    if(character_component)
      m_CharacterBody = character_component->get_character_body();

    // TODO: Do the checking and assignment only during weapon_change state
    if(weapon_component)
      m_CurrentWeapon = weapon_component->get_current_weapon_data();
  
    m_WeaponSwayMult = m_CurrentWeapon->get_weapon_sway_mult();
    m_WeaponSwayResetValue = m_CurrentWeapon->get_weapon_sway_reset();
    m_IdleWeaponBobFreq = m_CurrentWeapon->get_idle_weapon_bob_freq();
    m_IdleWeaponBobAmp = m_CurrentWeapon->get_idle_weapon_bob_amp();
    m_IdleWeaponBobSmoothVal = m_CurrentWeapon->get_idle_weapon_bob_smooth_val();
  }

  void reset_weapon_sway(double delta)
  {
    m_HoldPointPos = hold_point_node->get_position();
    m_HoldPointPos.x = Math::lerp(m_HoldPointPos.x, 0.0f, m_WeaponSwayResetValue * (float)delta);
    m_HoldPointPos.y = Math::lerp(m_HoldPointPos.y, 0.0f, m_WeaponSwayResetValue * (float)delta);

    hold_point_node->set_position(m_HoldPointPos);
  }

  void weapon_idle_sway(double delta)
  {
    m_IdleWeaponBobTime += delta * 0.5f;

    float x_bob = Math::cos(m_IdleWeaponBobTime * m_IdleWeaponBobFreq * 0.5f) * m_IdleWeaponBobAmp;
    float y_bob = Math::sin(m_IdleWeaponBobTime * m_IdleWeaponBobFreq) * m_IdleWeaponBobAmp;

    Vector3 currentPos = hold_point_node->get_position();
    Vector3 newPos = Vector3(
      Utils::exp_decay(currentPos.x, x_bob, m_IdleWeaponBobSmoothVal, (float)delta),
      Utils::exp_decay(currentPos.y, y_bob, m_IdleWeaponBobSmoothVal, (float)delta), 
      0.0f
    );

    hold_point_node->set_position(newPos);
  }

  void weapon_sway(double delta, Vector2 sway_vector)
  {
    if(!m_CharacterBody || !hold_point_node) return;

    m_HoldPointPos = hold_point_node->get_position();
    m_HoldPointPos.x -= sway_vector.x * m_WeaponSwayMult * delta;
    m_HoldPointPos.y += sway_vector.y * m_WeaponSwayMult * delta;

    m_HoldPointPos = m_HoldPointPos.clamp(Vector3(-0.008f, -0.001f, 0.0f), Vector3(0.008f, 0.001f, 0.0f));

    hold_point_node->set_position(m_HoldPointPos);
  }

private:
  float m_WeaponSwayMult { 0.0f }, m_WeaponSwayResetValue { 0.0f };
  float m_IdleWeaponBobTime { 0.0f }, m_IdleWeaponBobFreq { 0.0f }, m_IdleWeaponBobAmp { 0.0f }, m_IdleWeaponBobSmoothVal;

  CharacterBody3D* m_CharacterBody { nullptr };
  Ref<Weapon> m_CurrentWeapon { nullptr }; // This is used to read the current weapon data set in the weapon component
  Vector3 m_HoldPointPos { Vector3(0.0f, 0.0f, 0.0f ) };
  
private:
  GD_DEFINE_PROPERTY(Node3D*, hold_point_node, nullptr);
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(WeaponComponent*, weapon_component, nullptr);
};
