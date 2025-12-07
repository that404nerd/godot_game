#include "weapon.h"

/*
    We create a weapon scene (for eg a pistol with anims, etc etc) and then we have a resource with packed scene,
    anim names which we fill up. Now in the Weapon Manager. We create a weapon node which is initialised inside the manager node. Now that Node3D
    contains animplayer and other stuff too. So we use the anim names from our res and load em.
*/

using namespace godot;

Weapon::Weapon()
{

}

void Weapon::_bind_methods() {

    ADD_GROUP("Weapon Transform Properties", "");
    GD_BIND_PROPERTY(Weapon, position, Variant::VECTOR3);
    GD_BIND_PROPERTY(Weapon, rotation, Variant::VECTOR3);
    GD_BIND_PROPERTY(Weapon, scale, Variant::VECTOR3);
    
    ADD_GROUP("Weapon Sway Properties", "");
    GD_BIND_PROPERTY(Weapon, swayMin, Variant::VECTOR2);
    GD_BIND_PROPERTY(Weapon, swayMax, Variant::VECTOR2);
    GD_BIND_PROPERTY(Weapon, swayWeaponPosMult, Variant::FLOAT);
    GD_BIND_PROPERTY(Weapon, swayWeaponRotMult, Variant::FLOAT);
    GD_BIND_PROPERTY(Weapon, swayWeaponPosLerp, Variant::FLOAT);
    GD_BIND_PROPERTY(Weapon, swayWeaponRotLerp, Variant::FLOAT);

    ADD_GROUP("Idle Sway Properties", "");
    GD_BIND_PROPERTY(Weapon, idleSwayAdjustment, Variant::FLOAT);
    GD_BIND_PROPERTY(Weapon, idleSwayRotationStrength, Variant::FLOAT);
    GD_BIND_PROPERTY(Weapon, randomSwayAmt, Variant::FLOAT);

    ADD_GROUP("Weapon Properties", "");
    GD_BIND_PROPERTY(Weapon, ammoCount, Variant::INT);
    GD_BIND_PROPERTY(Weapon, weaponName, Variant::STRING);
    GD_BIND_PROPERTY(Weapon, weaponEquipAnimName, Variant::STRING);
    GD_BIND_PROPERTY(Weapon, weaponShootingAnimName, Variant::STRING);

    ClassDB::bind_method(D_METHOD("get_weapon_model"), &Weapon::get_weapon_model);
    ClassDB::bind_method(D_METHOD("set_weapon_model", "weaponModel"), &Weapon::set_weapon_model);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "weapon_model", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_weapon_model", "get_weapon_model");
}

// Setters/Getters
Ref<PackedScene> Weapon::get_weapon_model() { return m_WeaponModel; }
void Weapon::set_weapon_model(const Ref<PackedScene>& weaponModel) { m_WeaponModel = weaponModel; }

Weapon::~Weapon() {}