#include "register_types.hpp"
#include "godot_cpp/core/class_db.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

  // the game runs, the checks are required prevents error spam (DO NOT CHANGE THE player from GDREGISTER_RUNTIME_CLASS)
  if (!ClassDB::class_exists("Game")) GDREGISTER_RUNTIME_CLASS(Game); 
  if (!ClassDB::class_exists("Player")) GDREGISTER_RUNTIME_CLASS(Player);
  if (!ClassDB::class_exists("CameraController")) GDREGISTER_RUNTIME_CLASS(CameraController); 
  
  if (!ClassDB::class_exists("PlayerState")) GDREGISTER_ABSTRACT_CLASS(PlayerState); 
  
  if (!ClassDB::class_exists("PlayerStateMachine")) GDREGISTER_RUNTIME_CLASS(PlayerStateMachine); // RUNTIME for other states to be valid
  if (!ClassDB::class_exists("PlayerIdleState")) GDREGISTER_CLASS(PlayerIdleState); 
  if (!ClassDB::class_exists("PlayerSprintState")) GDREGISTER_CLASS(PlayerSprintState); 
  if (!ClassDB::class_exists("PlayerJumpState")) GDREGISTER_CLASS(PlayerJumpState); 
  if (!ClassDB::class_exists("PlayerCrouchState")) GDREGISTER_CLASS(PlayerCrouchState); 
  if (!ClassDB::class_exists("PlayerDashState")) GDREGISTER_CLASS(PlayerDashState); 
  if (!ClassDB::class_exists("PlayerSlideState")) GDREGISTER_CLASS(PlayerSlideState); 
  if (!ClassDB::class_exists("PlayerFallingState")) GDREGISTER_CLASS(PlayerFallingState); 

  if (!ClassDB::class_exists("WeaponManager")) GDREGISTER_RUNTIME_CLASS(WeaponManager); // RUNTIME probably because the weapon anim is played in the editor (idk)
  if (!ClassDB::class_exists("Weapon")) GDREGISTER_CLASS(Weapon);
}


void uninitialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {

  // Initialization entry point Godot requires
  GDExtensionBool GDE_EXPORT GDExtensionInit(
          GDExtensionInterfaceGetProcAddress p_get_proc_address,
          const GDExtensionClassLibraryPtr p_library,
          GDExtensionInitialization *r_initialization
  ) {
      godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

      init_obj.register_initializer(initialize_module);
      init_obj.register_terminator(uninitialize_module);
      init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

      return init_obj.init();
  }

}

