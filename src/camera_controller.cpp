#include "camera_controller.h"

CameraController::CameraController() 
{
}

void CameraController::_ready()
{
  m_CharacterHead = character_component->get_character_head();
  m_OriginalControllerPos = get_position();

  m_OriginalFOV = character_camera->get_fov();
  sprint_fov = character_camera->get_fov() + 10.0f;
  slide_fov = character_camera->get_fov() + 20.0f;

}

void CameraController::_unhandled_input(const Ref<InputEvent>& event)
{
  Ref<InputEventMouseMotion> mouse_event = event;
  if(event->is_class("InputEventMouseMotion")) {

    character_component->rotate_y(-mouse_event->get_relative().x * character_component->get_mouse_sensitivity());
    character_camera->rotate_x(-mouse_event->get_relative().y * character_component->get_mouse_sensitivity());

    Vector3 camRot = character_camera->get_rotation();
    camRot.x = Math::clamp(camRot.x, Math::deg_to_rad(-89.0f), Math::deg_to_rad(89.0f));
    character_camera->set_rotation(Vector3(camRot.x, camRot.y, 0.0f));
  }
}

void CameraController::_bind_methods() 
{

  GD_BIND_CUSTOM_PROPERTY(CameraController, movement_manager, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CameraController, character_camera, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CameraController, character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(CameraController, movement_state_machine, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);

  ADD_GROUP("FOV Settings", "");
  GD_BIND_PROPERTY(CameraController, sprint_fov, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_fov, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_fov_zoom_in_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_fov_zoom_out_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_fov_zoom_in_transition_value, Variant::FLOAT);

  ADD_GROUP("Side Tilt Settings", "");
  GD_BIND_PROPERTY(CameraController, slide_tilt_angle, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, side_tilt_transition_value, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_tilt_rotation_transition, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, slide_start_timer, Variant::FLOAT);

  ADD_GROUP("Headbob Settings", "");
  GD_BIND_PROPERTY(CameraController, sprint_headbob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, sprint_headbob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, crouch_headbob_amp, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, crouch_headbob_freq, Variant::FLOAT);
  GD_BIND_PROPERTY(CameraController, headbob_transition_value, Variant::FLOAT);
}

void CameraController::_headbob_effect(double delta)
{
  bool onFloor = character_component->is_on_floor(); // so that bobbing doesn't occur during airborne states

  float velocity = character_component->get_velocity().length(); 
  m_HeadbobTime += velocity * onFloor * delta;

  float x_bob = Math::cos(m_HeadbobTime * sprint_headbob_freq * 0.5f) * sprint_headbob_amp; 
  float y_bob = Math::sin(m_HeadbobTime * sprint_headbob_freq) * sprint_headbob_amp;        

  if(m_CurrentStateID == static_cast<int>(MovementStates::CROUCH) && character_component->get_velocity().length() > 0.001f) {
    x_bob = Math::cos(m_HeadbobTime * crouch_headbob_freq * 0.5f) * crouch_headbob_amp; 
    y_bob = Math::sin(m_HeadbobTime * crouch_headbob_freq) * crouch_headbob_amp;        
  }

  Vector3 currentPos = m_CharacterHead->get_position();
  Vector3 newPos = Vector3(
    Utils::exp_decay(currentPos.x, x_bob, headbob_transition_value, (float)delta),
    Utils::exp_decay(currentPos.y, y_bob, headbob_transition_value, (float)delta), 
    0.0f
  );
  
  m_CharacterHead->set_position(newPos);
}

void CameraController::_tilt_player(double delta)
{
  Vector3 camControllerRot = get_rotation();

  if(m_CurrentStateID == static_cast<int>(MovementStates::SPRINT))
  {
    camControllerRot.z = Utils::exp_decay(camControllerRot.z, Math::deg_to_rad(side_tilt_angle) * -character_component->get_input_dir().x, side_tilt_transition_value, (float)delta);
  } else if(movement_manager->IsSliding())
  {
  }

  set_rotation(camControllerRot);
}

void CameraController::_slide_tilt(double delta)
{
  if(movement_manager->IsSliding())
  {
    if(m_SlideStartTimer >= 0.0f)
      m_SlideStartTimer -= delta;
  
    if(m_SlideStartTimer <= 0.0f)
    {
      m_DampedSpring.CalcDampedSpringMotionParams(delta, 25.0f, 0.3f);
      m_DampedSpring.UpdateDampedSpringMotion(m_SlideTilt, m_SlideTiltVel, Vector3(0.0f, 0.0f, Math::deg_to_rad(slide_tilt_angle)));
    }
  }

  if(!movement_manager->IsSliding())
  {
    m_SlideTilt = Vector3(0.0f, 0.0f, 0.0f);
    set_rotation(m_SlideTilt);
    set_position(m_OriginalControllerPos);
    m_SlideStartTimer = slide_start_timer;
  }

  set_rotation(m_SlideTilt);
}

void CameraController::_apply_fov(double delta)
{
  if(m_CurrentStateID == static_cast<int>(MovementStates::SPRINT))
  {
    character_camera->set_fov(Math::lerp(m_OriginalFOV, sprint_fov, sprint_fov_zoom_out_transition_value * (float)delta));
  } else if(m_CurrentStateID == static_cast<int>(MovementStates::SLIDE))
  {
    character_camera->set_fov(Math::lerp(character_camera->get_fov(), slide_fov, slide_fov_zoom_in_transition_value * (float)delta));
  } else {
    character_camera->set_fov(Math::lerp(character_camera->get_fov(), m_OriginalFOV, sprint_fov_zoom_out_transition_value * (float)delta));
  }
}

void CameraController::_physics_process(double delta) 
{
  m_CurrentStateID = movement_state_machine->get_current_state();

  _apply_fov(delta);
   
  _tilt_player(delta);

  _slide_tilt(delta); 

  if(m_CurrentStateID == static_cast<int>(MovementStates::SPRINT) || m_CurrentStateID == static_cast<int>(MovementStates::CROUCH))
  {
    _headbob_effect(delta);
  }

}

CameraController::~CameraController() {}
