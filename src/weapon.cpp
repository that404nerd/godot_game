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

  ADD_GROUP("Weapon Properties", "");
  GD_BIND_PROPERTY(Weapon, ammoCount, Variant::INT);
  GD_BIND_PROPERTY(Weapon, weaponName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, weaponEquipAnimName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, weaponUnequipAnimName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, weaponShootingAnimName, Variant::STRING);
  GD_BIND_PROPERTY(Weapon, weaponReloadAnimName, Variant::STRING);

  GD_BIND_CUSTOM_PROPERTY(Weapon, weaponScene, Variant::OBJECT, godot::PROPERTY_HINT_RESOURCE_TYPE);
  GD_BIND_CUSTOM_PROPERTY(Weapon, weaponAnimPlayer, Variant::OBJECT, godot::PROPERTY_HINT_NODE_TYPE);
}

Weapon::~Weapon() {}
