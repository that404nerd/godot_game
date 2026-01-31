#pragma once

#include "godot_cpp/variant/vector2.hpp"
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include "globals.h"

using namespace godot;

class Weapon : public Resource {
    GDCLASS(Weapon, Resource)

public:
    Weapon();

    ~Weapon();
protected: 
    static void _bind_methods();

private:

    GD_DEFINE_PROPERTY(Vector3, position, Vector3(0.0f, 0.0f, 0.0f));
    GD_DEFINE_PROPERTY(Vector3, rotation, Vector3(0.0f, 0.0f, 0.0f));
    GD_DEFINE_PROPERTY(Vector3, scale, Vector3(0.0f, 0.0f, 0.0f));

    GD_DEFINE_PROPERTY(Vector2, swayMin, Vector2(-20.0f, -20.0f));
    GD_DEFINE_PROPERTY(Vector2, swayMax, Vector2(20.0f, 20.0f));
    GD_DEFINE_PROPERTY(float, swayWeaponPosMult, 0.1f);
    GD_DEFINE_PROPERTY(float, swayWeaponRotMult, 30.0f);
    GD_DEFINE_PROPERTY(float, swayWeaponPosLerp, 0.07f);
    GD_DEFINE_PROPERTY(float, swayWeaponRotLerp, 0.1f);

    GD_DEFINE_PROPERTY(float, idleSwayAdjustment, 10.0f); // Adjust the sway when in the idle state
    GD_DEFINE_PROPERTY(float, idleSwayRotationStrength, 300.0f); // Adjust the sway rot while in the idle state
    GD_DEFINE_PROPERTY(float, randomSwayAmt, 5.0f);

    GD_DEFINE_PROPERTY(String, weaponName, "");
    GD_DEFINE_PROPERTY(String, weaponEquipAnimName, "");
    GD_DEFINE_PROPERTY(String, weaponShootingAnimName, "");

    GD_DEFINE_PROPERTY(int, ammoCount, 0);
    GD_DEFINE_PROPERTY(Ref<PackedScene>, weaponScene, nullptr);
};
