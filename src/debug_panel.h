#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

#include "globals.h"

using namespace godot;

class DebugPanel : public PanelContainer {
  GD_CLASS(DebugPanel, PanelContainer);

protected:
  static void _bind_methods();
    
public:
  void _ready() override;

  void add_debug_property(const String& title, const String& value);

  void _physics_process(double delta) override;
  void _input(const Ref<InputEvent>& event) override;

  ~DebugPanel();
private:
  Label* m_Property = nullptr;
  VBoxContainer* m_PropertyContainer = nullptr;
  String m_FPS = "";
};
