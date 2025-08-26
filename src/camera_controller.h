#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

namespace godot {

  class CameraController : public Node3D {
    GDCLASS(CameraController, Node3D)

  protected:
    static void _bind_methods();

  public:
    CameraController();
    ~CameraController();

    void _ready() override;

    void _process(double delta) override;
  
  };

}
