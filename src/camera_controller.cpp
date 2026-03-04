#include "camera_controller.h"
#include "globals.h"
#include "godot_cpp/core/math.hpp"
#include "player.h"
#include "player_state_machine.h"

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
  m_FinalFOV = m_PlayerCamera->get_fov() + 5.0f;

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
  GD_BIND_PROPERTY(CameraController, fov_zoom_out_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, fov_zoom_in_transition_value, Variant::FLOAT);
}


void CameraController::_physics_process(double delta) 
{
  m_CurrentState = m_StateMachineInst->get_current_state();

  if(m_CurrentState == StringName("Sprint"))
  {
    m_PlayerCamera->set_fov(Utils::exp_decay(m_OriginalFOV, m_FinalFOV, fov_zoom_out_transition_value, delta));
  } else if(m_CurrentState == StringName("Idle")) {
    m_PlayerCamera->set_fov(Utils::exp_decay(m_PlayerCamera->get_fov(), m_OriginalFOV, fov_zoom_in_transition_value, delta));
  }

  Vector3 camControllerRot = get_rotation();
  if(Math::abs(m_PlayerInst->get_input_dir().x) == 1.0f)
  {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, Math::deg_to_rad(5.0f) * -m_PlayerInst->get_input_dir().x, 10.0f, (float)delta);
  } else {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, 0.0f, 10.0f, (float)delta);
  }

  set_rotation(camControllerRot);


}

CameraController::~CameraController() {}
