#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "game.h"
#include "camera/camera_controller.h"
#include "player.h"

#include "managers/movement_manager.h"
#include "managers/weapon_manager.h"
#include "state_machines/state_machine.h"
#include "state_machines/movement_state_machine.h"
#include "state_machines/weapon_state_machine.h"
#include "state_machines/ai_state_machine.h"

#include "input_command_system.h"
#include "singletons/event_bus.h"
#include "resources/weapon.h"

#include "components/weapon_effects_components.h"
#include "components/ai_character_component.h"
#include "components/input_component.h"
#include "components/muzzle_flash_component.h"
#include "components/weapon_wrapper.h"

using namespace godot;

void initailize(ModuleInitializationLevel p_level);
void uinitailize(ModuleInitializationLevel p_leevel);
