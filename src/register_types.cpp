#include "register_types.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

static EventBus *s_EventBus = nullptr;

void initialize_module(ModuleInitializationLevel p_level) {

	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

  if(!ClassDB::class_exists("EventBus")) GDREGISTER_CLASS(EventBus);
  s_EventBus = memnew(EventBus);
  Engine::get_singleton()->register_singleton("EventBus", s_EventBus);

  // the game runs, the checks are required prevents error spam (DO NOT CHANGE THE Game from GDREGISTER_RUNTIME_CLASS)
  if(!ClassDB::class_exists("Game")) GDREGISTER_RUNTIME_CLASS(Game); 

  /*
    CharacterComponent is registered first because Player inherits from CharacterComponent
    and for some reason, the properties of CharacterComponent aren't shown in the editor if the order is switched 
  */
  if(!ClassDB::class_exists("CharacterComponent")) GDREGISTER_CLASS(CharacterComponent);
  if(!ClassDB::class_exists("Player")) GDREGISTER_CLASS(Player);
  
  if(!ClassDB::class_exists("CameraController")) GDREGISTER_RUNTIME_CLASS(CameraController); 

  if(!ClassDB::class_exists("Weapon")) GDREGISTER_CLASS(Weapon);
  if(!ClassDB::class_exists("CameraWeaponEffects")) GDREGISTER_RUNTIME_CLASS(CameraWeaponEffects);

  if(!ClassDB::class_exists("StateMachine")) GDREGISTER_RUNTIME_CLASS(StateMachine);
  if(!ClassDB::class_exists("MovementStateMachine")) GDREGISTER_RUNTIME_CLASS(MovementStateMachine);
  if(!ClassDB::class_exists("WeaponStateMachine")) GDREGISTER_RUNTIME_CLASS(WeaponStateMachine);
  if(!ClassDB::class_exists("WeaponManager")) GDREGISTER_RUNTIME_CLASS(WeaponManager);
  if(!ClassDB::class_exists("MovementManager")) GDREGISTER_RUNTIME_CLASS(MovementManager);

  // Components
  if(!ClassDB::class_exists("WeaponComponent")) GDREGISTER_CLASS(WeaponComponent);
  if(!ClassDB::class_exists("MuzzleFlashComponent")) GDREGISTER_CLASS(MuzzleFlashComponent);
  if(!ClassDB::class_exists("WeaponWrapper")) GDREGISTER_CLASS(WeaponWrapper);
}


void uninitialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

  Engine::get_singleton()->unregister_singleton("EventBus");

  if(s_EventBus)
  {
    memdelete(s_EventBus);
    s_EventBus = nullptr;
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

