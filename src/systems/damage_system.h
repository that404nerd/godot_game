#pragma once

#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/physical_bone_simulator3d.hpp>

#include "../components/hitbox_component.h"
#include "../components/physical_bone_hitbox.h"
#include "../components/health_component.h"
#include "../globals.h"

using namespace godot;

class DamageSystem : public Node
{
  GDCLASS(DamageSystem, Node);
public:
  void _ready() override;

  void _on_hit(float totalDamage, Node* hitSource, Node* hitTarget);

public:
  enum HitBoxType
  {
    BoneHitbox, GenericHitbox
  };

protected:
  static void _bind_methods();

private:
  GD_DEFINE_PROPERTY(HitBoxType, hitboxType, HitBoxType::BoneHitbox);

  GD_DEFINE_PROPERTY(PhysicalBoneSimulator3D*, ragdoll_simulator, nullptr);
  GD_DEFINE_PROPERTY(HitBoxComponent*, genericHitboxComp, nullptr);
  GD_DEFINE_PROPERTY(HealthComponent*, healthComp, nullptr);
};
VARIANT_ENUM_CAST(DamageSystem::HitBoxType);