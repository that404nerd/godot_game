#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label.hpp>

#include "player.h"

using namespace godot;

namespace godot {

  class CameraController : public Node3D {
    GDCLASS(CameraController, Node3D)

  protected:
    static void _bind_methods();

  public:
    CameraController();
    ~CameraController();

    void _unhandled_input(const Ref<InputEvent>& event) override;

    void _ready() override;
    void _process(double delta) override;
  
  private:
    Player* m_PlayerController = nullptr;

    Vector2 m_MouseInput;
    Vector3 m_InputRotation;

  };

}
