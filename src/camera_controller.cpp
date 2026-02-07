#include "camera_controller.h"

CameraController::CameraController() {
}

void CameraController::_ready()
{
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();
  m_HoldPointNode = get_node<Node3D>(NodePath("%WeaponHoldPoint"));
}

void CameraController::_unhandled_input(const Ref<InputEvent>& event)
{
  // Set the event to an mouse input event
  Ref<InputEventMouseMotion> mouse_event = event;
  if(event->is_class("InputEventMouseMotion")) {
    m_MouseInput.x += -mouse_event->get_screen_relative().x * m_PlayerInst->get_mouse_sensitivity();
    m_MouseInput.y += -mouse_event->get_screen_relative().y * m_PlayerInst->get_mouse_sensitivity();

    _weapon_sway(m_MouseInput);
  }
}

void CameraController::_bind_methods() 
{
  GD_BIND_PROPERTY(CameraController, sway_mult, Variant::FLOAT);
}

void CameraController::_weapon_sway(Vector2 sway_vector)
{
  Vector3 m_HoldPointPos = m_HoldPointNode->get_position();
  m_HoldPointPos.x -= sway_vector.x * sway_mult;
  m_HoldPointPos.y += sway_vector.y * sway_mult;
  m_HoldPointNode->set_position(m_HoldPointPos);
}

void CameraController::_physics_process(double delta) 
{
  Vector3 m_HoldPointPos = m_HoldPointNode->get_position();
  m_HoldPointPos.x = Math::lerp(m_HoldPointNode->get_position().x, 0.0f, (float)delta * 5.0f);
  m_HoldPointPos.y = Math::lerp(m_HoldPointNode->get_position().y, 0.0f, (float)delta * 5.0f);
  m_HoldPointNode->set_position(m_HoldPointPos);

  m_InputRotation.x = Math::clamp(m_InputRotation.x + m_MouseInput.y, Math::deg_to_rad(-89.0f), Math::deg_to_rad(89.0f));
  m_InputRotation.y += m_MouseInput.x;

  Transform3D pitchTransform = m_PlayerInst->get_camera_anchor()->get_transform();
  pitchTransform.basis = Basis::from_euler(Vector3(m_InputRotation.x, 0.0f, 0.0f));
  m_PlayerInst->get_camera_anchor()->set_transform(pitchTransform);

  Transform3D yawTransform = m_PlayerInst->get_global_transform();
  yawTransform.basis = Basis::from_euler(Vector3(0.0f, m_InputRotation.y, 0.0f));
  m_PlayerInst->set_transform(yawTransform);

  set_global_transform(m_PlayerInst->get_camera_anchor()->get_global_transform_interpolated());
  m_MouseInput = Vector2(0.0f, 0.0f);
}

CameraController::~CameraController() {}
