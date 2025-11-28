#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label.hpp>


using namespace godot;

namespace godot {

  class Game : public Node3D {
    GDCLASS(Game, Node3D)

  protected:
    static void _bind_methods();

  public:
    Game();
    ~Game();

    void _ready() override;
    void _unhandled_input(const Ref<InputEvent>& event) override;
    void _process(double delta) override;

  private:
    Label* m_FPSLabel = nullptr;
  
  };

}
