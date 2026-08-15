#include "vision_component.h"
#include "../ai_character_component.h"

void VisionComponent::_init()
{
  m_PlayerInst = Object::cast_to<Player>(get_tree()->get_first_node_in_group("player"));

  detection_area->connect("body_entered", Callable(this, "_on_player_entered_area"));
  detection_area->connect("body_exited", Callable(this, "_on_player_exited_area"));

  for(auto& raycast : eye_raycasts)
  {
    if(raycast)
    {
      RayCast3D* eyeRaycast = get_node<RayCast3D>(raycast);
      eyeRaycast->set_target_position(Vector3(0.0f, ai_character_component->get_vision_trigger_dist(), 0.0f)); 
    } 
  }
}

void VisionComponent::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, ai_character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, character_skeleton, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, detection_area, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, eye_raycasts, Variant::ARRAY, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_PROPERTY(VisionComponent, character_bone_name, Variant::STRING_NAME);

  GD_BIND_PROPERTY(VisionComponent, vision_fov, Variant::FLOAT);

  ClassDB::bind_method(D_METHOD("_on_player_entered_area", "body"), &VisionComponent::_on_player_entered_area);
  ClassDB::bind_method(D_METHOD("_on_player_exited_area", "body"), &VisionComponent::_on_player_exited_area);
}

void VisionComponent::_on_player_entered_area(Node* body)
{
  m_IsInArea = true;
}

void VisionComponent::_on_player_exited_area(Node* body)
{
  m_IsInArea = false;
}

void VisionComponent::_update_component_transform()
{
  int boneID = character_skeleton->find_bone(character_bone_name);

  if(boneID == -1)
  {
    print_error("Bone not found!");
  }

  m_BoneTransform = character_skeleton->get_bone_global_pose(boneID);
  
  Transform3D final_bone_transform = character_skeleton->get_global_transform() * m_BoneTransform;
  
  set_global_transform(final_bone_transform);
  set_scale(Vector3(1.0f, 1.0f, 1.0f));
  set_global_rotation(Vector3(final_bone_transform.basis.get_euler().x, final_bone_transform.basis.get_euler().y, 0.0f));
}

void VisionComponent::_update(double delta)
{
  _update_component_transform();

  if(m_IsInArea)
  {
    m_ForwardVector = (ai_character_component->get_global_basis().get_column(2)).normalized();
    m_PlayerPos = (m_PlayerInst->get_global_position() - ai_character_component->get_global_position()).normalized();

    float dot = m_ForwardVector.dot(m_PlayerPos);
    
    if(dot >= Math::cos(Math::deg_to_rad(vision_fov)))
    {
      
    } else {
    }
  }
}

void VisionComponent::_physics_update(double delta)
{

}