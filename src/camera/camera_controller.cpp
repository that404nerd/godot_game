#include "camera_controller.h"
#include "godot_cpp/variant/variant.hpp"

CameraController::CameraController() 
{
}

void CameraController::_ready()
{
  m_CharacterHead = character_component->get_character_head();

  m_OriginalFOV = character_camera->get_fov();
  m_BaseRot = get_rotation();
}

void CameraController::_unhandled_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouse_event = event;
  if(event->is_class("InputEventMouseMotion")) {

    character_component->rotate_y(-mouse_event->get_relative().x * character_component->get_mouse_sensitivity());
    character_camera->rotate_x(-mouse_event->get_relative().y * character_component->get_mouse_sensitivity());

    Vector3 camRot = character_camera->get_rotation();
    camRot.x = Math::clamp(camRot.x, Math::deg_to_rad(-89.0f), Math::deg_to_rad(89.0f));
    character_camera->set_rotation(Vector3(camRot.x, camRot.y, 0.0f));
  }
}

void CameraController::_bind_methods() 
{
  GD_BIND_CUSTOM_PROPERTY(CameraController, movement_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CameraController, character_camera, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CameraController, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ADD_GROUP("FOV Settings", "");
  GD_BIND_PROPERTY(CameraController, sprint_fov, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_fov, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_fov_zoom_in_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_fov_zoom_out_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_fov_zoom_in_transition_value, Variant::FLOAT);

  ADD_GROUP("Side Tilt Rot Settings", "");
  GD_BIND_PROPERTY(CameraController, slide_tilt_angle, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, side_tilt_transition_value, Variant::FLOAT);

  ADD_GROUP("Side Tilt Rot Settings", "");
  GD_BIND_PROPERTY(CameraController, slide_start_timer, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_tilt_start_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_tilt_start_damping_ratio, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_tilt_end_ang_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_tilt_end_damping_ratio, Variant::FLOAT);

  ADD_GROUP("Headbob Settings", "");
  GD_BIND_PROPERTY(CameraController, sprint_headbob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_headbob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, crouch_headbob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, crouch_headbob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, headbob_transition_value, Variant::FLOAT);
}

void CameraController::_headbob_effect(double delta)
{
  bool onFloor = character_component->is_on_floor(); // so that bobbing doesn't occur during airborne states

  float velocity = character_component->get_velocity().length(); 
  m_HeadbobTime += velocity * onFloor * delta;

  float x_bob = Math::cos(m_HeadbobTime * sprint_headbob_freq * 0.5f) * sprint_headbob_amp; 
  float y_bob = Math::sin(m_HeadbobTime * sprint_headbob_freq) * sprint_headbob_amp;        

  if(movement_manager->IsCrouching() && character_component->get_velocity().length() > 0.001f) {
    x_bob = Math::cos(m_HeadbobTime * crouch_headbob_freq * 0.5f) * crouch_headbob_amp; 
    y_bob = Math::sin(m_HeadbobTime * crouch_headbob_freq) * crouch_headbob_amp;        
  }

  Vector3 currentPos = m_CharacterHead->get_position();
  m_HeadBobPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, headbob_transition_value, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, headbob_transition_value, (float)delta), 
    0.0f
  );
  
}

void CameraController::_tilt_player(double delta)
{
  m_SideTiltRot = get_rotation();

  if(!movement_manager->IsSprinting())
  {
    m_SideTiltRot = Vector3(0.0f, 0.0f, 0.0f);
    return;
  }

  if(movement_manager->IsSprinting())
  {
    m_SideTiltRot.z = Utils::exp_decay(m_SideTiltRot.z, Math::deg_to_rad(side_tilt_angle) * -character_component->get_input_dir().x, side_tilt_transition_value, (float)delta);
  }
}

void CameraController::_slide_tilt(double delta)
{
  if(movement_manager->IsSliding())
  {
    if(m_SlideStartTimer >= 0.0f)
      m_SlideStartTimer -= delta;
  
    if(m_SlideStartTimer <= 0.0f)
    {
      m_DampedSpring.CalcDampedSpringMotionParams(delta, slide_tilt_start_ang_freq, slide_tilt_start_damping_ratio);
      m_DampedSpring.UpdateDampedSpringMotion(m_SlideTiltRot, m_SlideTiltRotVel, Vector3(0.0f, 0.0f, Math::deg_to_rad(slide_tilt_angle)));
    }
  }

  if(!movement_manager->IsSliding())
  {
    m_DampedSpring.CalcDampedSpringMotionParams(delta, slide_tilt_end_ang_freq, slide_tilt_end_damping_ratio);
    m_DampedSpring.UpdateDampedSpringMotion(m_SlideTiltRot, m_SlideTiltRotVel, Vector3(0.0f, 0.0f, 0.0f));

    m_SlideStartTimer = slide_start_timer;
  }
}

void CameraController::_apply_fov(double delta)
{
  float camFov = 0.0f;

  if(movement_manager->IsSprinting() && Math::abs(character_component->get_input_dir().x) != 1.0f)
  {
    camFov = sprint_fov;
  } else if(movement_manager->IsSliding())
  {
    camFov = slide_fov;
  } else {
    camFov = m_OriginalFOV;
  }

  character_camera->set_fov(Utils::exp_decay(character_camera->get_fov(), camFov, sprint_fov_zoom_out_transition_value, (float)delta));
}

void CameraController::_physics_process(double delta) 
{
  _apply_fov(delta);
  _tilt_player(delta);
  _slide_tilt(delta); 
  _headbob_effect(delta);

  m_FinalPos = m_BasePos + m_HeadBobPos;
  m_FinalRot = m_BaseRot + m_SideTiltRot + m_SlideTiltRot;

  m_CharacterHead->set_position(m_FinalPos);
  set_rotation(m_FinalRot);
}

CameraController::~CameraController() {}