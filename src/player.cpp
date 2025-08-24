#include "player.h"

/*
    TODO: Have a seperate gravity when falling
*/

Player::Player()
{
}

void Player::_bind_methods()
{
}

void Player::_ready()
{
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
    
    m_PlayerVel = get_velocity();
    
    m_PlayerHead = get_node<Node3D>(NodePath("PlayerRot/PlayerHead"));
    m_PlayerRotNode = get_node<Node3D>(NodePath("PlayerRot"));
    m_PlayerCamera = get_node<Camera3D>(NodePath("PlayerRot/PlayerHead/Camera3D"));

    m_JumpBufferTimer = get_node<Timer>(NodePath("JumpBufferTimer"));
    
    m_StandingCollisionShape = get_node<CollisionShape3D>(NodePath("StandingCollisionShape"));
    
    m_RaycastUp = get_node<RayCast3D>(NodePath("RayCastUp"));
    m_RaycastLeft = get_node<RayCast3D>(NodePath("RayCastLeft"));
    m_RaycastRight = get_node<RayCast3D>(NodePath("RayCastRight"));
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{

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

bool Player::_noclip(double delta) 
{
    if(Input::get_singleton()->is_action_pressed("noclip") && OS::get_singleton()->has_feature("debug")) {
        m_IsNoClip = !m_IsNoClip;
    } else {
        return false;
    }

    m_StandingCollisionShape->set_disabled(true);
    // m_CrouchingCollisionShape->set_disabled(true);

    float noclip_speed = Globals::SprintSpeed * 3.0f;

    m_PlayerVel = m_CamWishDir * noclip_speed;
    global_translate(m_PlayerVel * delta);
    set_velocity(m_PlayerVel);

    return true;
}

// Depends on the horizontal velocity
void Player::headbob_effect(double delta)
{
    Vector3 playerVelHeadbob = Vector3(m_PlayerVel.x, 0.0f, m_PlayerVel.z); // Use this vector so that y-vel (gravity) doesn't affect the headbob
    m_HeadBobTime += playerVelHeadbob.length() * delta;

    Transform3D headbobTransform = m_PlayerHead->get_transform(); // get the player's transform
    headbobTransform.origin = Vector3( // like a sine wave
        Math::cos(m_HeadBobTime * Globals::HEADBOB_FREQUENCY) * Globals::HEADBOB_MOVE_AMOUNT,
        Math::sin(m_HeadBobTime * Globals::HEADBOB_FREQUENCY) * Globals::HEADBOB_MOVE_AMOUNT,
        0.0f
    );
    m_PlayerHead->set_transform(headbobTransform);
}

void Player::_handle_ground_physics(double delta) 
{
    float currentSpeedInWishDir = m_PlayerVel.dot(m_WishDir);
    float addSpeed = get_move_speed() - currentSpeedInWishDir;

    if(addSpeed > 0.0f) {
        float accel = m_GroundAccel * get_move_speed() * delta;
        accel = Math::min(accel, addSpeed);
        m_PlayerVel += accel * m_WishDir;
    }

    // Friciton code
    float control = Math::max(m_PlayerVel.length(), m_GroundDecel); // Dont let speed to drop to zero instead to ground decl when stopping
    float drop = control * m_GroundFriction * delta; // how much velocity should be dropped due to friction
    float newSpeed = Math::max(m_PlayerVel.length() - drop, 0.0f); // New speed has to be subtracted from the current velocity due to friction

    if(m_PlayerVel.length() > 0.0f) {
        newSpeed /= m_PlayerVel.length();
    }
    
    m_PlayerVel *= newSpeed;

    if (!Math::is_equal_approx(m_InputDir.x, 0.0f) && is_on_floor()) {
        float targetTilt = (m_InputDir.x > 0 ? -Globals::SideTiltAngle : Globals::SideTiltAngle);
        m_PlayerTiltVector.z = Math::lerp(m_PlayerTiltVector.z, Math::deg_to_rad(targetTilt), (float)delta * 5.0f);
    } else {
        m_PlayerTiltVector.z = Math::lerp(m_PlayerTiltVector.z, 0.0f, (float)delta * 5.0f);
    }
        
    m_PlayerRotNode->set_rotation(m_PlayerTiltVector);

    
    headbob_effect(delta);
    set_velocity(m_PlayerVel);
    
}

// TODO: Do crouch logic
void Player::_handle_crouch(double delta) {
    // if(Input::get_singleton()->is_action_just_pressed("crouch")) {
    //     m_IsCrouching = true;
    // } else if(m_IsCrouching && test_move(get_transform(), Vector3(0.0f, 2.0f, 0.0f))) {
    //     m_IsCrouching = false;
    // }

    // Vector3 crouchPosition;
    // crouchPosition.y = Math::move_toward(m_PlayerHead->get_position().y, m_IsCrouching ? -5.0f : 0.0f, (float)delta * 5.0f);
    // m_PlayerHead->set_position(crouchPosition);
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
   m_WishDir = this->get_global_transform().basis.xform(Vector3(m_InputDir.x, 0.0f, m_InputDir.y)).normalized();
   m_CamWishDir = m_PlayerCamera->get_global_transform().basis.xform(Vector3(m_InputDir.x, 0.0f, m_InputDir.y)).normalized();
   
   // Set the jump state
   if(Input::get_singleton()->is_action_just_pressed("jump")) {
       m_JumpBufferTimer->start();
    } else if(Input::get_singleton()->is_action_just_released("jump")) {
        m_IsFalling = true;
    }
    
    _handle_crouch(delta);
    
    if(m_IsFalling) {
        m_PlayerVel.y -= Globals::DOWN_GRAVITY * delta;
    }

    if(m_CurrentJumps > 0 && m_JumpBufferTimer->get_time_left() > 0.0f) {
        m_CurrentJumps--;
        m_PlayerVel.y = Globals::JumpSpeed;
        m_JumpBufferTimer->stop();
    } 
    
    if(!_noclip(delta)) {
        if(is_on_floor()) {
            m_CurrentJumps = Globals::MAX_JUMPS; 
            _handle_ground_physics(delta);
            m_IsFalling = false;
        } else {
            _handle_air_physics(delta);
        }
        
        move_and_slide();
    }
}


Player::~Player()
{
    
}