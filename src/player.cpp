#include "player.h"

Player::Player()
{
    
}

void Player::_bind_methods()
{
    ADD_GROUP("Player Speed Settings", "");
    GD_BIND_PROPERTY(Player, crouch_speed, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, sprint_speed, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, dash_speed, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, slide_speed, Variant::FLOAT);
  
    ADD_GROUP("Player Jump Settings", "");
    GD_BIND_PROPERTY(Player, jump_height, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, time_to_peak, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, time_to_descent, Variant::FLOAT);
  
    ADD_GROUP("Player Air Strafe Settings", "");
    GD_BIND_PROPERTY(Player, max_air_move_speed, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, max_air_accel, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, mouse_sensitivity, Variant::FLOAT);

    ADD_GROUP("Player Friction Settings", "");
    GD_BIND_PROPERTY(Player, ground_accel, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, ground_decel, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, ground_friction, Variant::FLOAT);

    ADD_GROUP("Player Misc Settings", "");
    GD_BIND_PROPERTY(Player, crouch_translate, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, lerp_constant, Variant::FLOAT);
    GD_BIND_PROPERTY(Player, slide_tilt_angle, Variant::FLOAT);
}

void Player::_ready()
{
    m_CameraControllerNode = get_node<Node3D>(NodePath("CameraController"));
    
    m_PlayerHead = get_node<Node3D>(NodePath("CameraController/PlayerHead"));
    m_PlayerCamera = get_node<Camera3D>(NodePath("CameraController/PlayerHead/Camera3D"));
    m_WeaponCamera = get_node<Camera3D>(NodePath("CameraController/PlayerHead/Camera3D/WeaponViewportContainer/WeaponViewport/WeaponCamera"));

    m_WeaponSubViewport = get_node<SubViewport>(NodePath("CameraController/PlayerHead/Camera3D/WeaponViewportContainer/WeaponViewport"));
    m_WeaponSubViewport->set_size(DisplayServer::get_singleton()->window_get_size());

    m_CameraAnchor = get_node<Marker3D>(NodePath("CameraControllerAnchor")); 

    m_StandingPlayerCollider = get_node<CollisionShape3D>(NodePath("StandingPlayerCollider"));
    m_CrouchingPlayerCollider = get_node<CollisionShape3D>(NodePath("CrouchingPlayerCollider"));

    m_PlayerMeshInst = get_node<MeshInstance3D>(NodePath("PlayerMesh"));
    m_PlayerCapsule = Object::cast_to<CapsuleMesh>(m_PlayerMeshInst);

    m_Gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{
    
}

void Player::_update_input() 
{
    Vector3 playerVel = get_velocity();
    m_WeaponCamera->set_global_transform(m_PlayerCamera->get_global_transform());

    m_InputDir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized();
    
    /*
        global_transform - position/rotation/scale of the player relative to the world
        Basis - rotation & scale (no translation) of the player
        Everything that rotates the player (mouse look, aim, head tilt, body rotation) modifies the basis.
        basis.xform(vector) - convert the local direction to world spaced direction
        Ignore the y-coordinate since that affects gravity
    */
    m_WishDir = get_global_transform().basis.xform(Vector3(m_InputDir.x, 0.0f, m_InputDir.y)).normalized();
    
    if (is_on_floor())
    {
        if (m_WishDir != Vector3(0.0f, 0.0f, 0.0f))
        {
            playerVel.x = Math::lerp(playerVel.x, m_WishDir.x, 0.0f);
            playerVel.z = Math::lerp(playerVel.z, m_WishDir.z, 0.0f);
        }
        else
        {
            playerVel.x = Math::lerp(playerVel.x, 0.0f, 0.3f);
            playerVel.z = Math::lerp(playerVel.z, 0.0f, 0.3f);
        }
    }

    set_velocity(playerVel);
}

void Player::_update_velocity()
{
    move_and_slide();
}

void Player::_physics_process(double delta) 
{
}


Player::~Player()
{
}
