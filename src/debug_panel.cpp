#include "debug_panel.h"

void DebugPanel::_ready()
{
  m_PropertyContainer = get_node<VBoxContainer>("MarginContainer/VBoxContainer");
  add_debug_property("FPS", m_FPS);
}

void DebugPanel::_bind_methods()
{

}

void DebugPanel::_input(const Ref<InputEvent>& event)
{
  
}

void DebugPanel::add_debug_property(const String& title, const String& value)
{
  m_Property = memnew(Label());
  m_PropertyContainer->add_child(m_Property);
  m_Property->set_name(title); 
  m_Property->set_text(m_Property->get_name() + value);
}


void DebugPanel::_physics_process(double delta)
{
  m_FPS = String::num(Engine::get_singleton()->get_frames_per_second());
  if (m_Property) {
    m_Property->set_text(m_Property->get_text() + ": " + m_FPS);
  }
}

DebugPanel::~DebugPanel()
{
}