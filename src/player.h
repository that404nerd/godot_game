#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "components/character_component.h"

#include "globals.h"
#include "global_state_handler.h"
#include "weapon_manager.h"

using namespace godot;

class Player : public CharacterComponent {
  GDCLASS(Player, CharacterComponent);

private:

  struct PlayerGlobalState {
    bool CanDash = true;
    float DashCooldown;
  };

public:
  Player();
  
  void _ready() override;
  void _process(double delta) override;
  void _physics_process(double delta) override;
  void _unhandled_input(const Ref<InputEvent>& event) override;

  static void _bind_methods();
  
  ~Player();
  
public:

  Camera3D* get_player_camera() { return m_PlayerCamera; }

  Node3D* get_camera_controller() { return m_CameraControllerNode; }

  PlayerGlobalState& get_global_state() { return m_GlobalState; }

private:
  PlayerGlobalState m_GlobalState;

  Node3D* m_CameraControllerNode { nullptr };
  
  Camera3D* m_PlayerCamera { nullptr };
  
  Node3D* m_CamController { nullptr };

  GlobalStateHandler* m_GlobalStateHandler { nullptr };
};
