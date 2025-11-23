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

    Ref<PackedScene> get_weapon_model();
    void set_weapon_model(const Ref<PackedScene>& weaponModel);

    ~Weapon();
protected: 
    static void _bind_methods();

private:

    GD_DEFINE_PROPERTY(Vector2, swayMin, Vector2(-20.0f, -20.0f));
    GD_DEFINE_PROPERTY(Vector2, swayMax, Vector2(20.0f, 20.0f));
    GD_DEFINE_PROPERTY(Vector2, position, Vector2(0.0f, 0.0f));
    GD_DEFINE_PROPERTY(Vector2, rotation, Vector2(0.0f, 0.0f));

    GD_DEFINE_PROPERTY(float, swayWeaponPosMult, 0.1f);
    GD_DEFINE_PROPERTY(float, swayWeaponRotMult, 30.0f);
    GD_DEFINE_PROPERTY(float, swayWeaponPosLerp, 0.07f);
    GD_DEFINE_PROPERTY(float, swayWeaponRotLerp, 0.1f);

    GD_DEFINE_PROPERTY(String, weaponName, String());
    GD_DEFINE_PROPERTY(String, weaponEquipAnimName, String());
    GD_DEFINE_PROPERTY(String, weaponShootingAnimName, String());

    GD_DEFINE_PROPERTY(int, ammoCount, 0);

    Ref<PackedScene> m_WeaponModel;
};
