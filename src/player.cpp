#include "player.h"

/* 
    TODO: What's the horizontal velocity?
*/

Player::Player()
{
}

void Player::_bind_methods()
{
}

void Player::_ready()
{
    
    m_PlayerVel = get_velocity();
    
    m_JumpBufferTimer = Globals::MAX_JUMP_BUFFER_TIME;

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
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

    // Set the event to an mouse input event
    Ref<InputEventMouseMotion> mouse_event = event;
    if(event->is_class("InputEventMouseMotion")) {
        rotate_y(-mouse_event->get_relative().x * Globals::MouseSensitivity); // Set the yaw (turns the player left or right)
        m_PlayerHead->rotate_x(-mouse_event->get_relative().y * Globals::MouseSensitivity); // Set the pitch to the camera (up or down)

        // Clamp the pitch so that we dont look straight up
        Vector3 rot = m_PlayerHead->get_rotation();
        rot.x = Math::clamp(rot.x, Math::deg_to_rad(-90.0f), Math::deg_to_rad(90.0f));
        m_PlayerHead->set_rotation(rot);
    }
}

// Depends on the horizontal velocity
void Player::headbob_effect(double delta)
{
    Vector3 playerVelHeadbob = Vector3(m_PlayerVel.x, 0.0f, m_PlayerVel.z); // Use this vector so that y-vel (gravity) doesn't affect the headbob
    m_HeadBobTime += playerVelHeadbob.length() * delta;

    Transform3D headbobTransform = m_PlayerHead->get_transform(); // get the player's transform
    headbobTransform.origin = Vector3(
        Math::cos(m_HeadBobTime * Globals::HEADBOB_FREQUENCY) * Globals::HEADBOB_MOVE_AMOUNT,
        Math::sin(m_HeadBobTime * Globals::HEADBOB_FREQUENCY) * Globals::HEADBOB_MOVE_AMOUNT,
        0.0f
    );
    m_PlayerHead->set_transform(headbobTransform);
}

void Player::_handle_ground_physics(double delta) 
{
    m_PlayerVel.x = m_WishDir.x * get_move_speed();
    m_PlayerVel.z = m_WishDir.z * get_move_speed();
    set_velocity(m_PlayerVel);
    
    headbob_effect(delta);
}

void Player::_handle_air_physics(double delta) {
    
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
    // Apply gravity
    float gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");
    if(!is_on_floor()) {
        m_PlayerVel.y -= gravity * delta;
    }
    
    Vector2 input_dir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized();
    
    /*
        global_transform - position/rotation/scale of the player relative to the world
        Basis - where the player is actually pointing to (3x3 rotation matrix)
        basis.xform(vector) - convert the local direction to world spaced direction
    */
    m_WishDir = get_transform().basis.xform(Vector3(input_dir.x, 0.0f, input_dir.y)).normalized();
       
       
    // Set the jump state
    if(Input::get_singleton()->is_action_just_pressed("jump")) {
        m_JumpBufferTimer = Globals::MAX_JUMP_BUFFER_TIME;
    } 
    
    if(m_JumpBufferTimer > 0.0f) {
        m_JumpBufferTimer -= delta;
    }
    
    // Handle the jump state
    if(m_CurrentJumps > 0 && m_JumpBufferTimer > 0.0f) {
        m_PlayerVel.y = Globals::JumpSpeed;
        m_CurrentJumps--;
        m_JumpBufferTimer = 0.0f;
    } 
        
    if(is_on_floor()) {
        m_CurrentJumps = Globals::MAX_JUMPS;
        _handle_ground_physics(delta);
    } else {
        _handle_air_physics(delta);
    }
    
    move_and_slide();
}


Player::~Player()
{
    
}
