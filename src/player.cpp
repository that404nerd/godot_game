#include "player.h"

Player::Player() : m_IsPressed(false)
{
}

void Player::_bind_methods()
{
}

void Player::_ready()
{
    m_PlayerVel = get_velocity();
    
    m_CameraControllerNode = get_node<Node3D>(NodePath("CameraController"));
    m_VelLabel = get_node<Label>(NodePath("TextDebugLayer/VelInfoLabel"));
    
    m_PlayerHead = get_node<Node3D>(NodePath("CameraController/PlayerHead"));
    m_PlayerCamera = get_node<Camera3D>(NodePath("CameraController/PlayerHead/Camera3D"));
    m_CameraAnchor = get_node<Marker3D>(NodePath("CameraControllerAnchor")); 

    m_JumpBufferTimer = get_node<Timer>(NodePath("JumpBufferTimer"));
    m_StandingPlayerCollider = get_node<CollisionShape3D>(NodePath("StandingPlayerCollider"));
    m_CrouchingPlayerCollider = get_node<CollisionShape3D>(NodePath("CrouchingPlayerCollider"));

    FStateManager::GetStateManagerInstance()._initialize_manager(*this);
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{
    FStateManager::GetStateManagerInstance().transition_states(*this, get_physics_process_delta_time(), event);

    if (event->is_action_pressed("toggle_sprint")) {
        m_IsPressed = !m_IsPressed;
    }
}

void Player::_physics_process(double delta) 
{
    m_InputDir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized();

    /*
        global_transform - position/rotation/scale of the player relative to the world
        Basis - rotation & scale (no translation) of the player
        Everything that rotates the player (mouse look, aim, head tilt, body rotation) modifies the basis.
        basis.xform(vector) - convert the local direction to world spaced direction
        Ignore the y-coordinate since that affects gravity
    */
    m_WishDir = get_global_transform().basis.xform(Vector3(m_InputDir.x, 0.0f, m_InputDir.y)).normalized();
   
    FStateManager::GetStateManagerInstance()._physics_update(delta, *this);
    String vel_text = "Velocity: " + String::num(get_velocity().length(), 2);
    m_VelLabel->set_text(vel_text);
    
    move_and_slide();
}


Player::~Player()
{
}