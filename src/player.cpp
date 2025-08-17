#include "player.h"

/* Some clarity ffs:
    MOUSE X  ──► rotate_y()  (Yaw)
    MOUSE Y  ──► rotate_x()  (Pitch)
*/

Player::Player() : m_PlayerVel(get_velocity())
{
}

void Player::_bind_methods()
{
}

void Player::_ready()
{
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

    m_PlayerCamera = get_node<Camera3D>(NodePath("PlayerHead/Camera3D"));

    m_PlayerHead = get_node<Node3D>(NodePath("PlayerHead"));
    m_DashTimer = get_node<Timer>(NodePath("DashTimer"));

    m_StandingCollisionShape = get_node<CollisionShape3D>(NodePath("StandingCollisionShape"));
    m_CrouchingCollisionShape = get_node<CollisionShape3D>(NodePath("CrouchingCollisionShape"));

    m_RaycastUp = get_node<RayCast3D>(NodePath("RayCastUp"));
    m_RaycastLeft = get_node<RayCast3D>(NodePath("RayCastLeft"));
    m_RaycastRight = get_node<RayCast3D>(NodePath("RayCastRight"));
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{
    if(Input::get_singleton()->get_mouse_mode() == Input::MOUSE_MODE_CAPTURED) {
        if(event->is_class("InputEventMouseMotion")) {

            // Set the event to an mouse input event
            Ref<InputEventMouseMotion> mouse_event = event;

            rotate_y(-mouse_event->get_relative().x * 0.006f); // Set the yaw (turns the player left or right)
            m_PlayerCamera->rotate_x(-mouse_event->get_relative().y * 0.006f); // Set the pitch to the camera (up or down)

            // Clamp the pitch so that we dont look straight up
            Vector3 rot = m_PlayerCamera->get_rotation();
            rot.x = Math::clamp(rot.x, Math::deg_to_rad(-89.0f), Math::deg_to_rad(89.0f));
            m_PlayerCamera->set_rotation(rot);
        }
    }
}

// Depends on the horizontal velocity
void Player::headbob_effect(double delta)
{
    Vector3 playerHorizontalVector = Vector3(m_PlayerVel.x, 0.0f, m_PlayerVel.z);
    m_HeadBobTime += delta * playerHorizontalVector.length();

    Transform3D headbobTransform = m_PlayerCamera->get_transform();
    headbobTransform.origin = Vector3(
        Math::cos(m_HeadBobTime * Globals::HEADBOB_FREQUENCY * 0.5f) * Globals::HEADBOB_MOVE_AMOUNT,
        Math::sin(m_HeadBobTime * Globals::HEADBOB_FREQUENCY) * Globals::HEADBOB_MOVE_AMOUNT,
        0.0f
    );
    m_PlayerCamera->set_transform(headbobTransform);
}

void Player::_handle_ground_physics(double delta) 
{
    m_PlayerVel.x = m_WishDir.x * get_move_speed();
    m_PlayerVel.z = m_WishDir.z * get_move_speed();
    set_velocity(m_PlayerVel);
    
    headbob_effect(delta);
}

void Player::_handle_air_physics(double delta)
{
    // Air strafing
    float currentSpeed = m_PlayerVel.dot(m_WishDir);
    float capped_speed = Math::min((Globals::AirMoveSpeed * m_WishDir).length(), Globals::AirCapSpeed);
    float add_speed = capped_speed - currentSpeed;

    if(add_speed > 0.0f) {
        print_line("Add speed is greater than 0");
        float acc = Globals::AirAccel * Globals::AirMoveSpeed * delta;
        acc = Math::min(acc, add_speed);
        m_PlayerVel += acc * m_WishDir;
    }
    set_velocity(m_PlayerVel);
}

void Player::_physics_process(double delta) 
{
    float gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");
    m_PlayerVel.y -= gravity * delta;
    Vector2 input_dir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized();
    /*
        global_transform - position/rotation/scale of the player relative to the world
        Basis - where the player is actually pointing to (3x3 rotation matrix)
        basis.xform(vector) - convert the local direction to world spaced direction
    */
    m_WishDir = get_global_transform().basis.xform(Vector3(input_dir.x, 0.0f, input_dir.y));

    if(is_on_floor()) {
        if(Input::get_singleton()->is_action_just_pressed("jump")) {
            m_PlayerVel.y = Globals::JumpSpeed;
        }
        _handle_ground_physics(delta);
    } else {
        _handle_air_physics(delta);
    }

    move_and_slide();
}


Player::~Player()
{
    
}
