#include "camera_controller.h"

CameraController::CameraController() 
{
}

void CameraController::_ready()
{
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();

  // This is really weird. You can't use the getter function from the player class 
  // instead you have to use the unique access identifier (if set obv) otherwise it will crash. I have no clue why.
  m_PlayerCamera = get_node<Camera3D>(NodePath("%PlayerCamera"));

  m_OriginalFOV = m_PlayerCamera->get_fov();
  sprint_fov = m_PlayerCamera->get_fov() + 10.0f;
  slide_fov = m_PlayerCamera->get_fov() + 20.0f;
}

void CameraController::_unhandled_input(const Ref<InputEvent>& event)
{
  // Set the event to an mouse input event
  Ref<InputEventMouseMotion> mouse_event = event;
  if(event->is_class("InputEventMouseMotion")) {

    m_PlayerInst->rotate_y(-mouse_event->get_relative().x * m_PlayerInst->get_mouse_sensitivity());
    m_PlayerCamera->rotate_x(-mouse_event->get_relative().y * m_PlayerInst->get_mouse_sensitivity());

    Vector3 camRot = m_PlayerCamera->get_rotation();
    camRot.x = Math::clamp(camRot.x, Math::deg_to_rad(-89.0f), Math::deg_to_rad(89.0f));
    m_PlayerCamera->set_rotation(Vector3(camRot.x, camRot.y, 0.0f));
  }
}

void CameraController::_bind_methods() 
{
  ADD_GROUP("FOV Settings", "");
  GD_BIND_PROPERTY(CameraController, sprint_fov, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_fov, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_fov_zoom_in_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_fov_zoom_out_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_fov_zoom_in_transition_value, Variant::FLOAT);

  ADD_GROUP("Side Tilt Settings", "");
  GD_BIND_PROPERTY(CameraController, slide_tilt_angle, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, side_tilt_angle, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, side_tilt_transition_value, Variant::FLOAT);

  ADD_GROUP("Headbob Settings", "");
  GD_BIND_PROPERTY(CameraController, sprint_headbob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_headbob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, crouch_headbob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, crouch_headbob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, headbob_delta_translate, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, headbob_transition_value, Variant::FLOAT);
}

void CameraController::_headbob_effect(double delta)
{
  bool onFloor = m_PlayerInst->is_on_floor(); // so that bobbing doesn't occur during airborne states

  float velocity = m_PlayerInst->get_velocity().length(); 
  m_HeadbobTime += delta * velocity * onFloor;

  float x_bob = Math::cos(m_HeadbobTime * sprint_headbob_freq * 0.5f) * sprint_headbob_amp; 
  float y_bob = Math::sin(m_HeadbobTime * sprint_headbob_freq) * sprint_headbob_amp;        

  if(m_CurrentState == StringName("Crouch")) {
    x_bob = Math::cos(m_HeadbobTime * crouch_headbob_freq * 0.5f) * crouch_headbob_amp; 
    y_bob = Math::sin(m_HeadbobTime * crouch_headbob_freq) * crouch_headbob_amp;        
  }

  Vector3 currentPos = m_PlayerInst->get_player_head()->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, headbob_transition_value, (float)delta * headbob_delta_translate),
    Utils::exp_decay(currentPos.y, y_bob, headbob_transition_value, (float)delta * headbob_delta_translate), 
    0.0f
  );
  
  // Otherwise don't apply headbob since without this condition it will set invalid transforms
  if(m_CurrentState == StringName("Sprint") || m_CurrentState == StringName("Crouch"))
  {
    m_PlayerInst->get_player_head()->set_position(newPos);
  }
}

void CameraController::_tilt_player(double delta)
{
  Vector3 camControllerRot = get_rotation();
  if(m_CurrentState == StringName("Sprint"))
  {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, Math::deg_to_rad(side_tilt_angle) * -m_PlayerInst->get_input_dir().x, side_tilt_transition_value, (float)delta);
  } else if(m_CurrentState == StringName("Slide"))
  {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, Math::deg_to_rad(side_tilt_angle), side_tilt_transition_value, (float)delta);
  } else {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, 0.0f, side_tilt_transition_value, (float)delta);
  }

  set_rotation(camControllerRot);
}

void CameraController::_physics_process(double delta) 
{
  m_CurrentState = m_StateMachineInst->get_current_state();


  if(m_CurrentState == StringName("Sprint"))
  {
    m_PlayerCamera->set_fov(Math::lerp(m_OriginalFOV, sprint_fov, sprint_fov_zoom_out_transition_value * (float)delta));
  } else if(m_CurrentState == StringName("Slide"))
  {
    m_PlayerCamera->set_fov(Math::lerp(m_PlayerCamera->get_fov(), slide_fov, slide_fov_zoom_in_transition_value * (float)delta));
  } else {
    m_PlayerCamera->set_fov(Math::lerp(m_PlayerCamera->get_fov(), m_OriginalFOV, sprint_fov_zoom_out_transition_value * (float)delta));
  } 
  
  _tilt_player(delta);
  
  _headbob_effect(delta);

}

CameraController::~CameraController() {}
