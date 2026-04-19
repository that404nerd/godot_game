#include "camera_controller.h"

CameraController::CameraController() 
{
}

void CameraController::_ready()
{
  m_PlayerInst = Object::cast_to<Player>(get_parent());
  m_PlayerCamera = get_node<Camera3D>(NodePath("%PlayerCamera"));

  m_WeaponHoldPoint = get_node<Node3D>(NodePath("%WeaponHoldPoint"));

  m_OriginalFOV = m_PlayerCamera->get_fov();
  sprint_fov = m_PlayerCamera->get_fov() + 10.0f;
  slide_fov = m_PlayerCamera->get_fov() + 20.0f;
}

void CameraController::_unhandled_input(const Ref<InputEvent>& event)
{
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
  bool onFloor = m_PlayerInst->is_on_floor(); // so that bobbing doesn't occur during airborne states

  float velocity = m_PlayerInst->get_velocity().length(); 
  m_HeadbobTime += velocity * onFloor * delta;

  float x_bob = Math::cos(m_HeadbobTime * sprint_headbob_freq * 0.5f) * sprint_headbob_amp; 
  float y_bob = Math::sin(m_HeadbobTime * sprint_headbob_freq) * sprint_headbob_amp;        

  if(m_CurrentStateID == static_cast<uint8_t>(PlayerStates::CROUCH) && m_PlayerInst->get_velocity().length() > 0.001f) {
    x_bob = Math::cos(m_HeadbobTime * crouch_headbob_freq * 0.5f) * crouch_headbob_amp; 
    y_bob = Math::sin(m_HeadbobTime * crouch_headbob_freq) * crouch_headbob_amp;        
  }

  Vector3 currentPos = m_PlayerInst->get_player_head()->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, headbob_transition_value, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, headbob_transition_value, (float)delta), 
    0.0f
  );
  
  m_PlayerInst->get_player_head()->set_position(newPos);
}

void CameraController::_tilt_player(double delta)
{
  Vector3 camControllerRot = get_rotation();
  Vector3 weaponHoldPointRot = m_WeaponHoldPoint->get_rotation();

  if(m_CurrentStateID == static_cast<uint8_t>(PlayerStates::SPRINT))
  {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, Math::deg_to_rad(side_tilt_angle) * -m_PlayerInst->get_input_dir().x, side_tilt_transition_value, (float)delta);

  } else if(m_CurrentStateID == static_cast<uint8_t>(PlayerStates::SLIDE))
  {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, Math::deg_to_rad(side_tilt_angle), side_tilt_transition_value, (float)delta);
    weaponHoldPointRot.z = Utils::exp_decay(m_WeaponHoldPoint->get_rotation().z, Math::deg_to_rad(slide_tilt_rotation), slide_tilt_rotation_transition, (float)delta);

  } else {
    weaponHoldPointRot.z = Utils::exp_decay(m_WeaponHoldPoint->get_rotation().z, 0.0f, slide_tilt_rotation_transition, (float)delta);
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, 0.0f, side_tilt_transition_value, (float)delta);
  }

  m_WeaponHoldPoint->set_rotation(weaponHoldPointRot);
  set_rotation(camControllerRot);
}

void CameraController::_apply_fov(double delta)
{
  if(m_CurrentStateID == static_cast<uint8_t>(PlayerStates::SPRINT))
  {
    m_PlayerCamera->set_fov(Math::lerp(m_OriginalFOV, sprint_fov, sprint_fov_zoom_out_transition_value * (float)delta));
  } else if(m_CurrentStateID == static_cast<uint8_t>(PlayerStates::SLIDE))
  {
    m_PlayerCamera->set_fov(Math::lerp(m_PlayerCamera->get_fov(), slide_fov, slide_fov_zoom_in_transition_value * (float)delta));
  } else {
    m_PlayerCamera->set_fov(Math::lerp(m_PlayerCamera->get_fov(), m_OriginalFOV, sprint_fov_zoom_out_transition_value * (float)delta));
  }
}

void CameraController::_physics_process(double delta) 
{
  /* I check if the player's is_node_ready() because the player's _ready()
     is called after the camera controller node's _ready() which means that setting this without
     any checks is a guaranteed nullptr and a crash if used */
  if(m_PlayerInst->is_node_ready() && m_PlayerInst != nullptr)
  {
    m_PlayerStateMachine = m_PlayerInst->get_player_state_machine();
  }

  m_CurrentStateID = m_PlayerStateMachine->get_current_state();

  _apply_fov(delta);
   
  _tilt_player(delta);
  
  if(m_CurrentStateID == static_cast<uint8_t>(PlayerStates::SPRINT) || m_CurrentStateID == static_cast<uint8_t>(PlayerStates::CROUCH))
  {
    _headbob_effect(delta);
  }

}

CameraController::~CameraController() {}
