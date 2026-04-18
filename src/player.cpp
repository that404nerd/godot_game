#include "player.h"
#include "player_state_machine.h"

Player::Player()
{
  get_global_state().DashCooldown = dash_cooldown;
  m_CharacterComponent.set_character_body_inst(this);
}

void Player::_bind_methods()
{
  ADD_GROUP("Player Speed Settings", "");
  GD_BIND_PROPERTY(Player, crouch_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Player, sprint_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Player, slide_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Player, dash_speed, Variant::FLOAT);

  ADD_GROUP("Player Jump Settings", "");
  GD_BIND_PROPERTY(Player, jump_height, Variant::FLOAT);
  GD_BIND_PROPERTY(Player, down_gravity, Variant::FLOAT);

  ADD_GROUP("Player Air Strafe Settings", "");
  GD_BIND_PROPERTY(Player, max_air_move_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Player, mouse_sensitivity, Variant::FLOAT);

  ADD_GROUP("Player Acceleration Settings", "");
  GD_BIND_PROPERTY(Player, ground_accel, Variant::FLOAT);
  GD_BIND_PROPERTY(Player, ground_decel, Variant::FLOAT);

  ADD_GROUP("Player Misc Settings", "");
  GD_BIND_PROPERTY(Player, crouch_translate_speed, Variant::FLOAT);
  GD_BIND_PROPERTY(Player, dash_cooldown, Variant::FLOAT);
  GD_BIND_PROPERTY(Player, crouch_translate, Variant::FLOAT);

}

void Player::_ready()
{
  m_PlayerStateMachine = memnew(PlayerStateMachine(this));
  m_GlobalStateHandler = memnew(GlobalStateHandler(this));
  m_WeaponManager = memnew(WeaponManager);

  m_PlayerStateMachine->_enter();
  m_GlobalStateHandler->_enter();

  m_WeaponHoldPoint = get_node<Node3D>(NodePath("%WeaponHoldPoint"));
<<<<<<< HEAD
  m_WeaponAnimPlayer = get_node<AnimationPlayer>(NodePath("%WeaponAnimPlayer"));

  m_WeaponManager->_init_data({ &m_CharacterComponent, m_WeaponHoldPoint, m_PlayerStateMachine, m_WeaponAnimPlayer });
=======

  m_WeaponManager->_init_data({ &m_CharacterComponent, m_WeaponHoldPoint, m_PlayerStateMachine });
>>>>>>> 55838bc (changed weapon anims)
  
  m_CameraControllerNode = get_node<Node3D>(NodePath("CameraController"));
  m_PlayerHead = get_node<Node3D>(NodePath("CameraController/PlayerHead"));
  m_PlayerCamera = get_node<Camera3D>(NodePath("%PlayerCamera"));
  m_ColliderRayCast = get_node<RayCast3D>(NodePath("PlayerRaycasts/PlayerColliderRay"));
  m_StandingPlayerCollider = get_node<CollisionShape3D>(NodePath("StandingPlayerCollider"));
  m_CrouchingPlayerCollider = get_node<CollisionShape3D>(NodePath("CrouchingPlayerCollider"));
}

void Player::_unhandled_input(const Ref<InputEvent>& event)
{
  m_PlayerStateMachine->_unhandled_input(event);
  m_WeaponManager->_unhandled_input(event);
}

void Player::_update_input() 
{
  Vector3 playerVel = get_velocity();

  m_InputDir = Input::get_singleton()->get_vector("left", "right", "forward", "back").normalized();
  m_WishDir = get_global_transform().basis.xform(Vector3(m_InputDir.x, 0.0f, m_InputDir.y)).normalized();
  
  if (is_on_floor())
  {
    if (m_WishDir != Vector3(0.0f, 0.0f, 0.0f))
    {
      playerVel.x = Utils::exp_decay(playerVel.x, m_WishDir.x, 15.0f, ground_accel);
      playerVel.z = Utils::exp_decay(playerVel.z, m_WishDir.z, 15.0f, ground_accel);
    }
    else
    {
      playerVel.x = Utils::exp_decay(playerVel.x, 0.0f, 1.0f, ground_decel);
      playerVel.z = Utils::exp_decay(playerVel.z, 0.0f, 1.0f, ground_decel);
    }
  }

  playerVel += m_GravityVec;

  set_velocity(playerVel);
}

void Player::_update_velocity()
{
  move_and_slide();
}

void Player::_process(double delta)
{
  m_WeaponManager->_update(delta);
}

void Player::_physics_process(double delta) 
{
  m_PlayerStateMachine->_physics_update(delta);
  m_GlobalStateHandler->_physics_update(delta);
}

Player::~Player()
{
  memfree(m_WeaponManager);
  memfree(m_PlayerStateMachine);
  memfree(m_GlobalStateHandler);
}
