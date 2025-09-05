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
    
    m_CurrentPlayerState = memnew(PlayerSprintState);
    
    m_JumpBufferTimer = get_node<Timer>(NodePath("JumpBufferTimer"));
    
    m_StandingCollisionShape = get_node<CollisionShape3D>(NodePath("StandingCollisionShape"));
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

    PlayerState* newState = m_CurrentPlayerState->_handle_input(event, *this);

    if(newState == nullptr || newState == m_CurrentPlayerState) return;

    // Actual transition
    memdelete(m_CurrentPlayerState);
    m_CurrentPlayerState = newState;
    m_CurrentPlayerState->_enter(*this);

}

void Player::_handle_air_physics(double delta) {
    
    float gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");
    if(!is_on_floor()) {
        m_PlayerVel.y -= gravity * delta;
    }
        
    // Air strafing
    float currentSpeed = m_PlayerVel.dot(m_WishDir);
    float addSpeed = Globals::MaxAirMoveSpeed - currentSpeed; // how much speed we can add to the player before exceeding max move speed in air
    
    
    if (addSpeed > 0.0f) {
        float accel = Globals::MaxAirAccel * addSpeed;
        m_PlayerVel += (m_WishDir * accel) * delta; // v = u + (a * t)
    }
    
    Vector3 playerHorizVel = Vector3(m_PlayerVel.x, m_PlayerVel.y, 0.0f); // The horizontal velocity of the player
    if(playerHorizVel.length() > Globals::MaxAirMoveSpeed) { // Check if we are exceeding the max move speed in air
        playerHorizVel = playerHorizVel.normalized() * Globals::MaxAirMoveSpeed;
    }
    
    // Set final velocity
    m_PlayerVel.x = playerHorizVel.x;
    m_PlayerVel.y = playerHorizVel.y;
    set_velocity(m_PlayerVel);
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

    if(Input::get_singleton()->is_action_just_pressed("jump")) {
       m_JumpBufferTimer->start();
    } else if(Input::get_singleton()->is_action_just_released("jump")) {
        m_IsFalling = true;
    }

    if(m_IsFalling) {
        m_PlayerVel.y -= Globals::DOWN_GRAVITY * delta;
    }

    // print_line("Current Movement State: ", get_current_movement_state());

    if(is_on_floor()) {

        if(m_JumpBufferTimer->get_time_left() > 0.0f) {
            m_PlayerVel.y = Globals::JumpSpeed;
            m_JumpBufferTimer->stop();
        } 

        m_CurrentPlayerState->_update(delta, *this);

    } else {
        
        _handle_air_physics(delta);
    }
    
    move_and_slide();
}


Player::~Player()
{
}