#include "camera_controller.h"

CameraController::CameraController() {
}

void CameraController::_ready()
{
    // Lifetime managed by godot
    m_PlayerInst = Object::cast_to<Player>(get_parent());
}

void CameraController::_unhandled_input(const Ref<InputEvent>& event)
{
    // Set the event to an mouse input event
    Ref<InputEventMouseMotion> mouse_event = event;
    if(event->is_class("InputEventMouseMotion")) {
        m_MouseInput.x += -mouse_event->get_screen_relative().x * m_PlayerInst->get_mouse_sensitivity();
        m_MouseInput.y += -mouse_event->get_screen_relative().y * m_PlayerInst->get_mouse_sensitivity();
    }
}

void CameraController::_bind_methods() {}

void CameraController::_physics_process(double delta) 
{
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
