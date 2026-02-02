#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label.hpp>

#include "globals.h"
#include "player_state_machine.h"
#include "player.h"
#include "player_state_machine.h"

using namespace godot;

namespace godot {

  class WeaponCamera : public Camera3D {
    GDCLASS(WeaponCamera, Camera3D);

  protected:
    static void _bind_methods();

  public:
    void _unhandled_input(const Ref<InputEvent>& event) override;

    void _ready() override;
    void _physics_process(double delta) override;


    void sway_weapon(Vector2 weapon_sway_amt);
  
  private:
    Node3D* m_HoldPointNode = nullptr;
    Vector3 m_HoldPointPos = Vector3(0.0f, 0.0f, 0.0f);
    
    Player* m_PlayerInst = nullptr;
    PlayerStateMachine* m_StateMachineInst = nullptr;

    GD_DEFINE_PROPERTY(float, sway_mult, 0.0f); 
  };

}
