#include "player.h"

Player::Player()
{
}

void Player::_bind_methods()
{
}

void Player::_ready()
{
    m_PlayerVel = get_velocity();
    
    m_CameraControllerNode = get_node<Node3D>(NodePath("CameraController"));
    
    m_PlayerHead = get_node<Node3D>(NodePath("CameraController/PlayerHead"));
    m_PlayerCamera = get_node<Camera3D>(NodePath("CameraController/PlayerHead/Camera3D"));
    m_CameraAnchor = get_node<Marker3D>(NodePath("CameraControllerAnchor")); 

    m_JumpBufferTimer = get_node<Timer>(NodePath("JumpBufferTimer"));
    
    m_StandingCollisionShape = get_node<CollisionShape3D>(NodePath("StandingCollisionShape"));

    FStateManager::GetStateManagerInstance()._initialize_manager();
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
    FStateManager::GetStateManagerInstance().toggle_states(event, *this);
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
   
    // if(m_IsFalling) {
    //     m_PlayerVel.y -= Globals::DOWN_GRAVITY * delta;
    // }
    FStateManager::GetStateManagerInstance().print_player_states();
    FStateManager::GetStateManagerInstance()._update(delta, *this);
    
    move_and_slide();
}


Player::~Player()
{
}