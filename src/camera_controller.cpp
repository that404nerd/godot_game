#include "camera_controller.h"

CameraController::CameraController() 
{
}

void CameraController::_ready()
{
  m_PlayerHead = character_component->get_character_head();
  m_PlayerCamera = get_node<Camera3D>(NodePath("%PlayerCamera"));

  m_OriginalFOV = m_PlayerCamera->get_fov();
  sprint_fov = m_PlayerCamera->get_fov() + 10.0f;
  slide_fov = m_PlayerCamera->get_fov() + 20.0f;
}

void CameraController::_unhandled_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouse_event = event;
  if(event->is_class("InputEventMouseMotion")) {

    character_component->rotate_y(-mouse_event->get_relative().x * character_component->get_mouse_sensitivity());
    m_PlayerCamera->rotate_x(-mouse_event->get_relative().y * character_component->get_mouse_sensitivity());

    Vector3 camRot = m_PlayerCamera->get_rotation();
    camRot.x = Math::clamp(camRot.x, Math::deg_to_rad(-89.0f), Math::deg_to_rad(89.0f));
    m_PlayerCamera->set_rotation(Vector3(camRot.x, camRot.y, 0.0f));
  }
}

void CameraController::_bind_methods() 
{
  GD_BIND_CUSTOM_PROPERTY(CameraController, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CameraController, movement_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CameraController, weapon_hold_point, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ADD_GROUP("FOV Settings", "");
  GD_BIND_PROPERTY(CameraController, sprint_fov, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_fov, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_fov_zoom_in_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_fov_zoom_out_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_fov_zoom_in_transition_value, Variant::FLOAT);

  ADD_GROUP("Side Tilt Settings", "");
  GD_BIND_PROPERTY(CameraController, slide_tilt_angle, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, side_tilt_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_tilt_rotation, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_tilt_rotation_transition, Variant::FLOAT);

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

  if(m_CurrentStateID == static_cast<int8_t>(MovementStates::CROUCH) && character_component->get_velocity().length() > 0.001f) {
    x_bob = Math::cos(m_HeadbobTime * crouch_headbob_freq * 0.5f) * crouch_headbob_amp; 
    y_bob = Math::sin(m_HeadbobTime * crouch_headbob_freq) * crouch_headbob_amp;        
  }

  Vector3 currentPos = m_PlayerHead->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, headbob_transition_value, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, headbob_transition_value, (float)delta), 
    0.0f
  );
  
  m_PlayerHead->set_position(newPos);
}

void CameraController::_tilt_player(double delta)
{
  Vector3 camControllerRot = get_rotation();
  Vector3 weaponHoldPointRot = weapon_hold_point->get_rotation();

  if(m_CurrentStateID == static_cast<int8_t>(MovementStates::SPRINT))
  {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, Math::deg_to_rad(side_tilt_angle) * -character_component->get_input_dir().x, side_tilt_transition_value, (float)delta);

  } else if(m_CurrentStateID == static_cast<int8_t>(MovementStates::SLIDE))
  {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, Math::deg_to_rad(side_tilt_angle), side_tilt_transition_value, (float)delta);
    weaponHoldPointRot.z = Utils::exp_decay(weaponHoldPointRot.z, Math::deg_to_rad(slide_tilt_rotation), slide_tilt_rotation_transition, (float)delta);

  } else {
    weaponHoldPointRot.z = Utils::exp_decay(weaponHoldPointRot.z, 0.0f, slide_tilt_rotation_transition, (float)delta);
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, 0.0f, side_tilt_transition_value, (float)delta);
  }

  weapon_hold_point->set_rotation(weaponHoldPointRot);
  set_rotation(camControllerRot);
}

void CameraController::_apply_fov(double delta)
{
  if(m_CurrentStateID == static_cast<int8_t>(MovementStates::SPRINT))
  {
    m_PlayerCamera->set_fov(Math::lerp(m_OriginalFOV, sprint_fov, sprint_fov_zoom_out_transition_value * (float)delta));
  } else if(m_CurrentStateID == static_cast<int8_t>(MovementStates::SLIDE))
  {
    m_PlayerCamera->set_fov(Math::lerp(m_PlayerCamera->get_fov(), slide_fov, slide_fov_zoom_in_transition_value * (float)delta));
  } else {
    m_PlayerCamera->set_fov(Math::lerp(m_PlayerCamera->get_fov(), m_OriginalFOV, sprint_fov_zoom_out_transition_value * (float)delta));
  }
}

void CameraController::_physics_process(double delta) 
{
  m_CurrentStateID = movement_state_machine->get_current_state();

  _apply_fov(delta);
   
  _tilt_player(delta);
  
  if(m_CurrentStateID == static_cast<int8_t>(MovementStates::SPRINT) || m_CurrentStateID == static_cast<int8_t>(MovementStates::CROUCH))
  {
    _headbob_effect(delta);
  }

}

CameraController::~CameraController() {}
