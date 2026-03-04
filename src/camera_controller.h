#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/camera3d.hpp>

#include <godot_cpp/variant/string.hpp>

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

  void _ready() override;
  void _physics_process(double delta) override;

private:
  Player* m_PlayerInst = nullptr;
  PlayerStateMachine* m_StateMachineInst = nullptr;

  Camera3D* m_PlayerCamera = nullptr;

  float m_OriginalFOV, m_FinalFOV;

  StringName m_CurrentState;

  GD_DEFINE_PROPERTY(float, fov_zoom_out_transition_value, 20.0f);
  GD_DEFINE_PROPERTY(float, fov_zoom_in_transition_value, 10.0f);
};
