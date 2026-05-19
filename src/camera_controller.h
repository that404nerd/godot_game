#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
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

  void _ready() override;
  void _physics_process(double delta) override;
  
  void _apply_fov(double delta);
  void _tilt_player(double delta);
  void _headbob_effect(double delta);

private:
  Player* m_PlayerInst = nullptr;

  float m_OriginalFOV;
  float m_HeadbobTime;

  int8_t m_CurrentStateID;

  GD_DEFINE_PROPERTY(float, sprint_fov_zoom_out_transition_value, 20.0f);
  GD_DEFINE_PROPERTY(float, sprint_fov_zoom_in_transition_value, 10.0f);
  GD_DEFINE_PROPERTY(float, slide_fov_zoom_in_transition_value, 10.0f);
  GD_DEFINE_PROPERTY(float, sprint_fov, 0.0f);
  GD_DEFINE_PROPERTY(float, slide_fov, 0.0f);
  
  GD_DEFINE_PROPERTY(float, slide_tilt_angle, 2.0f);
  GD_DEFINE_PROPERTY(float, side_tilt_angle, 2.0f);
  GD_DEFINE_PROPERTY(float, side_tilt_transition_value, 15.0f);

  GD_DEFINE_PROPERTY(float, sprint_headbob_amp, 0.04f);
  GD_DEFINE_PROPERTY(float, sprint_headbob_freq, 2.0f);
  GD_DEFINE_PROPERTY(float, headbob_transition_value, 2.0f);
  GD_DEFINE_PROPERTY(float, crouch_headbob_amp, 0.04f);
  GD_DEFINE_PROPERTY(float, crouch_headbob_freq, 2.0f);

  GD_DEFINE_PROPERTY(float, slide_tilt_rotation, 10.0f);
  GD_DEFINE_PROPERTY(float, slide_tilt_rotation_transition, 10.0f);

private:
  Camera3D* m_PlayerCamera { nullptr };
  Node3D* m_PlayerHead { nullptr };

  GD_DEFINE_PROPERTY(PlayerStateMachine*, player_state_machine, nullptr);
  GD_DEFINE_PROPERTY(Node3D*, weapon_hold_point, nullptr);
};
