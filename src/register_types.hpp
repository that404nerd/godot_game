#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "limbo_register_types.h"
#include "geqo_register_types.h"

#include "game.h"
#include "camera/camera_controller.h"
#include "player.h"

#include "managers/movement_manager.h"
#include "managers/weapon_manager.h"
#include "managers/ai_manager.h"

#include "states/ai_states.h"
#include "state_machines/state_machine.h"
#include "state_machines/movement_state_machine.h"
#include "state_machines/weapon_state_machine.h"

#include "singletons/event_bus.h"
#include "singletons/event_bus.h"

#include "input_command_system.h"

#include "resources/ai_behaviour_props.h"
#include "resources/weapon.h"

#include "components/ai/vision_component.h"
#include "components/ai/detection_area_component.h"
#include "components/ai/ai_character_component.h"
#include "components/ai/move_to_target.h"

#include "components/weapon_effects_components.h"
#include "components/input_component.h"
#include "components/muzzle_flash_component.h"
#include "components/weapon_wrapper.h"
#include "components/ai/vision_component.h"

using namespace godot;

void initailize(ModuleInitializationLevel p_level);
void uinitailize(ModuleInitializationLevel p_leevel);
