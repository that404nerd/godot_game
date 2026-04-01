#include "weapon_state.h"

WeaponState::WeaponState()
{
}

void WeaponState::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_weapon_equip"), &WeaponState::_weapon_equip);
  ClassDB::bind_method(D_METHOD("_weapon_unequip", "nextWeaponName"), &WeaponState::_weapon_unequip);
  ClassDB::bind_method(D_METHOD("_weapon_shoot"), &WeaponState::_weapon_shoot);
  ClassDB::bind_method(D_METHOD("_weapon_reload"), &WeaponState::_weapon_reload);
  ClassDB::bind_method(D_METHOD("_weapon_change", "nextWeaponName"), &WeaponState::_weapon_change);

  ClassDB::bind_method(D_METHOD("_on_animation_finished", "anim_name"), &WeaponState::_on_animation_finished);
}

void WeaponState::_input(const Ref<InputEvent>& event)
{
  for(int i = 0; i < m_WeaponManager->get_weaponList().size(); i++)
  {
    String inputAction = "weapon_" + String::num(i + 1, 0); // INFO: Need to match the set input action in the editor
    if(Input::get_singleton()->is_action_just_pressed(inputAction))
    {
      m_WeaponManager->set_weapon_index(i);
      m_TempWeapon = m_WeaponManager->get_weaponList()[m_WeaponManager->get_weapon_index()];
      m_NextWeaponName = m_TempWeapon->get_weaponName();

      m_WeaponManager->emit_signal("weapon_unequip", m_NextWeaponName);
    }
  }

}

void WeaponState::_ready()
{
  m_WeaponManager = GameManager::get_singleton()->get_weapon_manager_inst();
  m_WeaponManager->connect("weapon_equip", Callable(this, "_weapon_equip"));
  m_WeaponManager->connect("weapon_unequip", Callable(this, "_weapon_unequip"));
  m_WeaponManager->connect("weapon_shoot", Callable(this, "_weapon_shoot"));
  m_WeaponManager->connect("weapon_reload", Callable(this, "_weapon_reload"));
  m_WeaponManager->connect("weapon_change", Callable(this, "_weapon_change"));

  m_WeaponAnimPlayer = get_node<AnimationPlayer>(NodePath("%WeaponAnimPlayer"));
  m_WeaponAnimPlayer->connect("animation_finished", Callable(this, "_on_animation_finished"));
  
  // Make this part of the resource file
  m_LoadScene = ResourceLoader::get_singleton()->load("res://assets/decals/bullet_decal.tscn");
  
  m_StateMachineInst = GameManager::get_singleton()->get_player_state_machine();
  m_PlayerInst = GameManager::get_singleton()->get_player_inst();

  _weapon_first_load();
}

// This initialises the starting weapon and plays the equip anima for it
void WeaponState::_weapon_first_load()
{
  m_CurrentWeapon = GameManager::get_singleton()->get_weapon_manager_inst()->get_current_weapon();
  m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount = m_CurrentWeapon->get_totalAmmoCount();
  m_WeaponManager->emit_signal("weapon_equip");
}

// TODO: Rewrite this
void WeaponState::_generate_decal()
{
  m_SpaceState = m_PlayerInst->get_player_camera()->get_world_3d()->get_direct_space_state();
  m_ScreenCenter = m_PlayerInst->get_player_camera()->get_viewport()->get_visible_rect().get_size() / 2.0f;
  Vector3 ray_start = m_PlayerInst->get_player_camera()->project_ray_origin(m_ScreenCenter);
  Vector3 ray_end = ray_start + (m_PlayerInst->get_player_camera()->project_ray_normal(m_ScreenCenter) *
                                m_WeaponManager->get_current_weapon()->get_gun_range());

  Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(ray_start, ray_end);
  query->set_collide_with_bodies(true);
  Dictionary result = m_SpaceState->intersect_ray(query);

  if(!result.is_empty())
  {
    Node* instance = m_LoadScene->instantiate();
    Decal* bulletDecal = Object::cast_to<Decal>(instance);
    get_tree()->get_root()->add_child(bulletDecal);
    bulletDecal->set_global_position(result["position"]);
    bulletDecal->look_at(bulletDecal->get_global_transform().origin + result["normal"], Vector3(0.0f, 1.0f, 0.0f));
    bulletDecal->rotate_object_local(Vector3(1.0f, 0.0f, 0.0f), 90.0f);
   }

}

void WeaponState::_weapon_shoot()
{
  if(m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount > 0)
  {
    if(m_WeaponAnimPlayer)
    {
      m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponShootingAnimName(), -1, m_CurrentWeapon->get_weaponAnimSpeedMultiplier());
    }

    _generate_decal();
    m_CurrentWeapon->get_weaponData_inst().BulletsConsumed = m_CurrentWeapon->get_weaponData_inst().BulletsConsumed + 1;
    print_line(m_CurrentWeapon->get_weaponData_inst().BulletsConsumed, " bullets consumed");
    m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount = m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount - 1;
  } else return;
}

void WeaponState::_weapon_reload()
{
  if((m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount != m_CurrentWeapon->get_totalAmmoCount() || m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount == 0))
  {
    if(m_WeaponAnimPlayer)
    {
      m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponReloadAnimName(), -1, m_CurrentWeapon->get_weaponAnimSpeedMultiplier());
    }
  
    // NOTE: Currently no ammo pickups...
    if(m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount != 0)
    {
      float weaponAmmo = m_CurrentWeapon->get_totalAmmoCount() - m_CurrentWeapon->get_weaponData_inst().BulletsConsumed;
      m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount = m_CurrentWeapon->get_weaponData_inst().BulletsConsumed + weaponAmmo;
      print_line("Set ", weaponAmmo, " ammo");
    } else if(m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount == 0) {
      m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount = m_CurrentWeapon->get_totalAmmoCount();
      print_line("Set complete ", m_CurrentWeapon->get_totalAmmoCount(), " ammo");
    }
  }

  m_CurrentWeapon->get_weaponData_inst().BulletsConsumed = 0;
}

void WeaponState::_weapon_change(const StringName& weaponName)
{
  int weapon_index = -1;

  for (int i = 0; i < m_WeaponManager->get_weaponList().size(); i++) {
    Ref<Weapon> res = m_WeaponManager->get_weaponList()[i]; 
    
    if (res.is_valid() && res->get_weaponName() == weaponName) {
      weapon_index = i;
      break;
    }
  }
  
  if(weapon_index != -1)
  {
    m_CurrentWeapon = m_WeaponManager->get_weaponList()[weapon_index];
    m_CurrentWeapon->get_weaponData_inst().CurrentAmmoCount = m_CurrentWeapon->get_totalAmmoCount() - m_CurrentWeapon->get_weaponData_inst().BulletsConsumed;
    m_NextWeaponName = "";

    m_WeaponManager->emit_signal("weapon_equip");
  }

}

void WeaponState::_on_animation_finished(const StringName& anim_name)
{
  if(anim_name == m_CurrentWeapon->get_weaponUnequipAnimName())
  {
    m_WeaponManager->emit_signal("weapon_change", m_NextWeaponName);
  }
}

void WeaponState::_weapon_unequip(const StringName& nextWeaponName)
{
  if(nextWeaponName != m_CurrentWeapon->get_weaponName())
  {
    if(m_WeaponAnimPlayer->get_current_animation() != m_CurrentWeapon->get_weaponUnequipAnimName())
    {
      m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponUnequipAnimName(), -1,
                                m_CurrentWeapon->get_weaponAnimSpeedMultiplier()); // Plays the unequip animation of the current weapon
      m_NextWeaponName = nextWeaponName;
    }
  }
}

void WeaponState::_weapon_equip()
{
  m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName(), -1, m_CurrentWeapon->get_weaponAnimSpeedMultiplier());
}

void WeaponState::_physics_process(double delta)
{
}
