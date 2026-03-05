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
  
  void _tilt_player(double delta);
  void _headbob_effect(double delta);

private:
  Player* m_PlayerInst = nullptr;
  PlayerStateMachine* m_StateMachineInst = nullptr;

  Camera3D* m_PlayerCamera = nullptr;

  float m_OriginalFOV;
  float m_HeadbobTime;

  StringName m_CurrentState;

  GD_DEFINE_PROPERTY(float, sprint_fov_zoom_out_transition_value, 20.0f);
  GD_DEFINE_PROPERTY(float, sprint_fov_zoom_in_transition_value, 10.0f);
  GD_DEFINE_PROPERTY(float, slide_fov_zoom_in_transition_value, 10.0f);
  GD_DEFINE_PROPERTY(float, sprint_fov, 0.0f);
  GD_DEFINE_PROPERTY(float, slide_fov, 0.0f);
  
  GD_DEFINE_PROPERTY(float, slide_tilt_angle, 2.0f);
  GD_DEFINE_PROPERTY(float, side_tilt_angle, 2.0f);
  GD_DEFINE_PROPERTY(float, side_tilt_transition_value, 15.0f);

  GD_DEFINE_PROPERTY(float, headbob_delta_translate, 20.0f);

  GD_DEFINE_PROPERTY(float, sprint_headbob_amp, 0.04f);
  GD_DEFINE_PROPERTY(float, sprint_headbob_freq, 2.0f);
  GD_DEFINE_PROPERTY(float, headbob_transition_value, 2.0f);
  GD_DEFINE_PROPERTY(float, crouch_headbob_amp, 0.04f);
  GD_DEFINE_PROPERTY(float, crouch_headbob_freq, 2.0f);

};
