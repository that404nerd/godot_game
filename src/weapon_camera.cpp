#include "weapon_camera.h"

void WeaponCamera::_ready()
{
    m_HoldPointNode = get_node<Node3D>(NodePath("HoldPoint"));
}

void WeaponCamera::_unhandled_input(const Ref<InputEvent>& event)
{
    Ref<InputEventMouseMotion> mouse_event = event;
    if(event->is_class("InputEventMouseMotion"))
    {
        sway_weapon(Vector2(-mouse_event->get_screen_relative().x * 0.003f, -mouse_event->get_screen_relative().y * 0.003f));
    }
}

void WeaponCamera::_bind_methods() {}

void WeaponCamera::_physics_process(double delta) 
{
    // Reset to 0.0, 0.0
    m_HoldPointPos = m_HoldPointNode->get_position();
    m_HoldPointPos.x = Math::lerp(m_HoldPointNode->get_position().x, 0.0f, (float)delta * 5.0f);
    m_HoldPointPos.y = Math::lerp(m_HoldPointNode->get_position().y, 0.0f, (float)delta * 5.0f);
    m_HoldPointNode->set_position(m_HoldPointPos);
}

void WeaponCamera::sway_weapon(Vector2 weapon_sway_amt)
{
    // Move the node that contains the rig using mouse motion
    // TODO: Need to rewrite some stuff!
    m_HoldPointPos = m_HoldPointNode->get_position();
    m_HoldPointPos.x -= weapon_sway_amt.x * 0.0008f;
    m_HoldPointPos.y += weapon_sway_amt.y * 0.0008f;
    m_HoldPointNode->set_position(m_HoldPointPos);
}
