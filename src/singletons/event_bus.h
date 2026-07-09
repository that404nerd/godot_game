#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class EventBus : public Object {
  GDCLASS(EventBus, Object);
  static EventBus* s_EventBus;

public:
  EventBus();

  static EventBus* get_singleton();

private:
  ~EventBus();

protected:
  static void _bind_methods();
};