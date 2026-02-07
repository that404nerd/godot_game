#pragma once

#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>

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

    GD_DEFINE_PROPERTY(String, weaponName, "");
    GD_DEFINE_PROPERTY(String, weaponEquipAnimName, "");
    GD_DEFINE_PROPERTY(String, weaponShootingAnimName, "");

    GD_DEFINE_PROPERTY(int, ammoCount, 0);
    GD_DEFINE_PROPERTY(AnimationPlayer*, weaponAnimPlayer, nullptr);
    GD_DEFINE_PROPERTY(Ref<PackedScene>, weaponScene, nullptr);
};
