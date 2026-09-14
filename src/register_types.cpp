#include "register_types.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

static EventBus *s_EventBus = nullptr;

void initialize_module(ModuleInitializationLevel p_level) {

  initialize_limboai_module(p_level);
  initialize_geqo_module(p_level);
  
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
	}
  
  GDREGISTER_CLASS(EventBus);
  s_EventBus = memnew(EventBus);
  Engine::get_singleton()->register_singleton("EventBus", EventBus::get_singleton());

  // the game runs, the checks are required prevents error spam (DO NOT CHANGE THE Game from GDREGISTER_RUNTIME_CLASS)
  GDREGISTER_RUNTIME_CLASS(Game); 

  /*
    CharacterComponent is registered first because Player inherits from CharacterComponent
    and for some reason, the properties of CharacterComponent aren't shown in the editor if the order is switched 
  */
  GDREGISTER_RUNTIME_CLASS(CharacterComponent);
  GDREGISTER_RUNTIME_CLASS(Player);
  
  GDREGISTER_RUNTIME_CLASS(CameraController); 

  GDREGISTER_CLASS(Weapon);
  GDREGISTER_CLASS(AIBehaviourProps);

  GDREGISTER_RUNTIME_CLASS(StateMachine);
  GDREGISTER_RUNTIME_CLASS(MovementStateMachine);
  GDREGISTER_RUNTIME_CLASS(WeaponStateMachine);
  GDREGISTER_RUNTIME_CLASS(AIStateMachine);
  GDREGISTER_CLASS(LimboTestState);

  GDREGISTER_RUNTIME_CLASS(WeaponManager);
  GDREGISTER_RUNTIME_CLASS(MovementManager);
  GDREGISTER_RUNTIME_CLASS(AIManager);

  GDREGISTER_RUNTIME_CLASS(InputCommandSystem);
  
  // Components
  GDREGISTER_RUNTIME_CLASS(InputComponent);
  GDREGISTER_RUNTIME_CLASS(WeaponEffects);
  GDREGISTER_RUNTIME_CLASS(WeaponActionEffects);
  GDREGISTER_CLASS(WeaponWrapper);
  GDREGISTER_CLASS(WeaponComponent);
  GDREGISTER_CLASS(MuzzleFlashComponent);

  GDREGISTER_RUNTIME_CLASS(AICharacterComponent);
  GDREGISTER_RUNTIME_CLASS(DetectionAreaComponent);
  GDREGISTER_RUNTIME_CLASS(LookAtPlayerComponent);
  GDREGISTER_RUNTIME_CLASS(VisionComponent);
}


void uninitialize_module(ModuleInitializationLevel p_level) {
  uninitialize_limboai_module(p_level);
  uninitialize_geqo_module(p_level);

	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

  Engine::get_singleton()->unregister_singleton("EventBus");
  memdelete(s_EventBus);
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