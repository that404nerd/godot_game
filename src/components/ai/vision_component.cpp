#include "vision_component.h"
#include "../ai/ai_character_component.h"

void VisionComponent::_init()
{
  m_PlayerInst = Object::cast_to<Player>(get_tree()->get_first_node_in_group("player"));

  m_RememberTime = max_remember_time;

  m_PlayerMarker = m_PlayerInst->get_character_marker();

  inner_detection_area->connect("body_entered", Callable(this, "_on_body_entered_inner_area"));
  inner_detection_area->connect("body_exited", Callable(this, "_on_body_exited_inner_area"));

  // for(int i = 0; i < ragdoll_simulator->get_children().size(); i++)
  // {
  //   PhysicalBone3D* bone = Object::cast_to<PhysicalBone3D>(ragdoll_simulator->get_children()[i]);
  //   m_BoneRIDs.append(bone->get_rid());
  // }
}

void VisionComponent::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_body_entered_inner_area", "body"), &VisionComponent::_on_body_entered_inner_area);
  ClassDB::bind_method(D_METHOD("_on_body_exited_inner_area", "body"), &VisionComponent::_on_body_exited_inner_area);

  GD_BIND_CUSTOM_PROPERTY(VisionComponent, AICharacterComponent, ai_character_component, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, DetectionAreaComponent, inner_detection_area, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, Marker3D, vision_marker, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(VisionComponent, PhysicalBoneSimulator3D, ragdoll_simulator, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  
  GD_BIND_PROPERTY(VisionComponent, can_debug, Variant::BOOL);
  GD_BIND_PROPERTY(VisionComponent, eye_fov, Variant::FLOAT);
  GD_BIND_PROPERTY(VisionComponent, eye_raycast_ray_rot, Variant::FLOAT);
  GD_BIND_PROPERTY(VisionComponent, eye_raycast_length, Variant::FLOAT);
  GD_BIND_PROPERTY(VisionComponent, max_remember_time, Variant::FLOAT);
}

void VisionComponent::_on_body_entered_inner_area(Node3D* body)
{
  if(body == m_PlayerInst)
    m_DidEnterInnerArea = true;
}

void VisionComponent::_on_body_exited_inner_area(Node3D* body)
{
  if(body == m_PlayerInst)
    m_DidEnterInnerArea = false;
}

void VisionComponent::_update_component_transform()
{
  set_global_transform(vision_marker->get_global_transform());
}

void VisionComponent::_update(double delta)
{
  _update_component_transform();

  if(m_RememberTime >= 0.0f)
    m_RememberTime -= delta;
}

void VisionComponent::_physics_update(double delta)
{
  /*
    NOTE: Since the AI has a PhysicalBoneSimulator3D and each bone from the Skeleton3D is connected to 
    the PhysicalBone3D in the PhysicalBoneSimulator3D, each bone except the head bone has it's own collision layer in this case 4.

    Now the reason for the head having no collision layer of 4 is because
  
  */


  Vector3 enemyForward = ai_character_component->get_basis().get_column(2).normalized();
  Vector3 toPlayerDirection = (m_PlayerInst->get_global_position() - ai_character_component->get_global_position()).normalized();

  float angle = Math::clamp(enemyForward.dot(toPlayerDirection.normalized()), -1.0f, 1.0f);
  
  if(m_DidEnterInnerArea)
  {
    // Check if player is in the given FOV
    if(Math::acos(angle) <= Math::deg_to_rad(eye_fov / 2.0f))
    {
      Vector3 eyeStart = vision_marker->get_global_transform().get_origin();
      Vector3 eyeEnd = m_PlayerMarker->get_global_position();

      if(can_debug)
      {
        DebugDraw3D::draw_sphere(eyeStart, 0.05f);
        DebugDraw3D::draw_sphere(m_PlayerMarker->get_global_position(), 0.1f);
        DebugDraw3D::draw_line(eyeStart, eyeEnd);
      }
    
      m_SpaceState = ai_character_component->get_world_3d()->get_direct_space_state();
      m_Query = PhysicsRayQueryParameters3D::create(eyeStart, eyeEnd, m_PlayerInst->get_collision_mask());
      // m_Query->set_exclude(m_BoneRIDs);

      m_CollisionResult = m_SpaceState->intersect_ray(m_Query);
     
      CollisionObject3D* collidedObj = Object::cast_to<CollisionObject3D>(m_CollisionResult["collider"]);

      if(collidedObj)
      {
        if(collidedObj == m_PlayerInst)
        {
          m_CanSeePlayer = true;
          m_RememberTime = max_remember_time; 
        } else {
          m_CanSeePlayer = false;
        }
      }
    }
  }

  if(m_RememberTime <= 0.0f)
    m_CanSeePlayer = false;
  
  // print_line("Vision Status: ", m_CanSeePlayer, ", Remember Time: ", m_RememberTime, " Max Remember Time: ", max_remember_time);
}

void VisionComponent::_exit_tree()
{

}