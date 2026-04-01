#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>

#include "weapon_manager.h"

using namespace godot;

class WeaponState : public Node
{
  GDCLASS(WeaponState, Node)
public:
  WeaponState();

  void _ready() override;
  void _input(const Ref<InputEvent>& event) override;
  void _physics_process(double delta) override;

public: 
  void _generate_decal();

  void _weapon_first_load();

  void _weapon_idle();
  void _weapon_equip();
  void _weapon_unequip(const StringName& nextWeaponName);
  void _weapon_shoot();
  void _weapon_reload();
  void _weapon_change(const StringName& weaponName);

private:
  void _on_animation_finished(const StringName& anim_name);

protected:
  static void _bind_methods();

private:
  WeaponManager* m_WeaponManager; 
  PlayerStateMachine* m_StateMachineInst;
  Player* m_PlayerInst;

  Ref<PackedScene> m_LoadScene;
  String m_NextWeaponName;

  AnimationPlayer* m_WeaponAnimPlayer = nullptr;
  PhysicsDirectSpaceState3D* m_SpaceState = nullptr;
  Ref<Weapon> m_CurrentWeapon, m_TempWeapon;
private:
  Vector2 m_ScreenCenter;
};
