#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>

#include "globals.h"
#include "weapon_manager.h"

enum class WeaponStates {
  IDLE, EQUIP, SHOOT, RELOAD, UNEQUIP, WEAPON_SWITCH, NONE = -1
};

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

  void _weapon_idle();
  void _weapon_equip();
  void _weapon_unequip();
  void _weapon_shoot();
  void _weapon_reload();
  void _weapon_change();

  void _process_states();

private:
  void _on_animation_finished(const StringName& anim_name);
  void _on_weapon_equip_anim_finished();
  void _on_weapon_unequip_anim_finished();

protected:
  static void _bind_methods();

private:
  AnimationPlayer* m_WeaponAnimPlayer = nullptr;
  Ref<Weapon> m_CurrentWeapon = nullptr;

  StringName m_CurrentStateName;
private:
  GD_DEFINE_PROPERTY(WeaponManager*, weapon_manager, nullptr);
};
