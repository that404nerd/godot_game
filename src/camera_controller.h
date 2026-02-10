#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/camera3d.hpp>

#include "globals.h"
#include "player_state_machine.h"
#include "player.h"

using namespace godot;

class CameraController : public Node3D {
  GDCLASS(CameraController, Node3D)

protected:
  static void _bind_methods();

public:
  CameraController();
  ~CameraController();

  void _unhandled_input(const Ref<InputEvent>& event) override;

  void _weapon_sway(Vector2 sway_vector);

  void _ready() override;
  void _physics_process(double delta) override;

private:
  Player* m_PlayerInst = nullptr;
  PlayerStateMachine* m_StateMachine = nullptr;

  Node3D* m_HoldPointNode = nullptr;

  GD_DEFINE_PROPERTY(float, sway_mult, 0.0f);

  Vector2 m_MouseInput;
  Vector3 m_InputRotation;
};
