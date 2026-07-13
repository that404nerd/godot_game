#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/camera3d.hpp>

#include "../globals.h"
#include "../movement_state_machine.h"

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

  void _slide_tilt(double delta);


private:
  float m_OriginalFOV { 0.0f };
  float m_HeadbobTime { 0.0f };

  GD_DEFINE_PROPERTY(float, sprint_fov_zoom_out_transition_value, 20.0f);
  GD_DEFINE_PROPERTY(float, sprint_fov_zoom_in_transition_value, 10.0f);
  GD_DEFINE_PROPERTY(float, slide_fov_zoom_in_transition_value, 10.0f);
  GD_DEFINE_PROPERTY(float, sprint_fov, 0.0f);
  GD_DEFINE_PROPERTY(float, slide_fov, 0.0f);
  
  GD_DEFINE_PROPERTY(float, side_tilt_angle, 2.0f);
  GD_DEFINE_PROPERTY(float, side_tilt_transition_value, 15.0f);

  GD_DEFINE_PROPERTY(float, slide_start_timer, 0.1f);
  GD_DEFINE_PROPERTY(float, slide_tilt_angle, 2.0f);
  GD_DEFINE_PROPERTY(float, slide_tilt_start_ang_freq, 25.0f);
  GD_DEFINE_PROPERTY(float, slide_tilt_start_damping_ratio, 0.3f);
  GD_DEFINE_PROPERTY(float, slide_tilt_end_ang_freq, 25.0f);
  GD_DEFINE_PROPERTY(float, slide_tilt_end_damping_ratio, 0.4f);

  GD_DEFINE_PROPERTY(float, sprint_headbob_amp, 0.04f);
  GD_DEFINE_PROPERTY(float, sprint_headbob_freq, 2.0f);
  GD_DEFINE_PROPERTY(float, headbob_transition_value, 2.0f);
  GD_DEFINE_PROPERTY(float, crouch_headbob_amp, 0.04f);
  GD_DEFINE_PROPERTY(float, crouch_headbob_freq, 2.0f);

private:
  float m_SlideStartTimer { slide_start_timer };

  Node3D* m_CharacterHead { nullptr };
  DampedSpring m_DampedSpring {};

  Vector3 m_BasePos, m_BaseRot {};
  Vector3 m_HeadBobPos {};

  Vector3 m_SideTiltRot {};

  Vector3 m_SlideTiltRot {}, m_SlideTiltRotVel {};

  Vector3 m_FinalPos {}, m_FinalRot {};
  
private:
  GD_DEFINE_PROPERTY(Camera3D*, character_camera, nullptr);
  GD_DEFINE_PROPERTY(CharacterComponent*, character_component, nullptr);
  GD_DEFINE_PROPERTY(MovementManager*, movement_manager, nullptr);
};
