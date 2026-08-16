#include "vision_component.h"
#include "../ai_character_component.h"

void VisionComponent::_init()
{
  m_PlayerInst = Object::cast_to<Player>(get_tree()->get_first_node_in_group("player"));

  m_RngGen.instantiate();
  _setup_vision_raycasts();

  m_RememberTime = max_remember_time;
}

void VisionComponent::_setup_vision_raycasts()
{
  
  for(int i = 0; i < eye_raycasts_count; i++)
  {
    /*
      - Even number of raycasts (preferably 16) [x]
      - Rotate raycast to -90deg in x-axis [x]
      - Set their target position to smtg like (0, -7 to -14, 0) [x]
      - Set Collision Mask to 1, 2 (Environment, Player respectively) [x]
      - Randomize their z-rotation from -20deg to 20deg in radians [x]

      The raycasts will not go out of the cone defined to match the volume the enemy can see
    */
    RayCast3D* raycast = memnew(RayCast3D);
    raycast->set_target_position(Vector3(0.0f, -eye_raycast_length, 0.0f));

    raycast->set_collision_mask_value(1, true);
    raycast->set_collision_mask_value(2, true);

    // Except the middle raycast all the other raycasts will have a random z-rotation b/w -20rad and 20rad.
    raycast->set_rotation(Vector3(Math::deg_to_rad(-90.0f), 0.0f, 
                              i == eye_raycasts_count / 2 ? 0.0f : m_RngGen->randf_range(Math::deg_to_rad(-5.0f), Math::deg_to_rad(5.0f))));

    add_child(raycast);
    m_VisionRayCasts.insert(i, raycast);
  }
  
}

void VisionComponent::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, ai_character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, character_skeleton, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, viewable_area, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_PROPERTY(VisionComponent, character_bone_name, Variant::STRING_NAME);

  GD_BIND_PROPERTY(VisionComponent, eye_raycast_length, Variant::FLOAT);
  GD_BIND_PROPERTY(VisionComponent, eye_raycasts_count, Variant::INT);
  GD_BIND_PROPERTY(VisionComponent, max_remember_time, Variant::FLOAT);
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

  if(m_RememberTime >= 0.0f)
    m_RememberTime -= delta;

  for(int i = 0; i < eye_raycasts_count; i++)
  {
    RayCast3D* raycast = Object::cast_to<RayCast3D>(m_VisionRayCasts[i]);
    m_Colliding = (raycast->get_collider_rid() == m_PlayerInst->get_rid());

    // Check if m_SawPlayer is false so that multiple rays in one sweep doesn't reset the state
    if(m_Colliding && !m_SawPlayer)
    {
      m_SawPlayer = true;
    }
  }

  if(m_RememberTime <= 0.0f)
  {
    m_SawPlayer = false;
  }  
  
  if(m_Colliding)
    m_RememberTime = max_remember_time; 

  print_line("Vision Status: ", m_SawPlayer, ", Remember Time: ", m_RememberTime, " Max Remember Time: ", max_remember_time);

}

void VisionComponent::_physics_update(double delta)
{

}

void VisionComponent::_exit_tree()
{

}