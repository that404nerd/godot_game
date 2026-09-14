#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "game.h"
#include "camera/camera_controller.h"
#include "player.h"

#include "managers/movement_manager.h"
#include "managers/weapon_manager.h"
#include "managers/ai_manager.h"

#include "state_machines/state_machine.h"
#include "state_machines/movement_state_machine.h"
#include "state_machines/weapon_state_machine.h"
#include "state_machines/ai_state_machine.h"

#include "singletons/event_bus.h"
#include "singletons/event_bus.h"

#include "input_command_system.h"

#include "resources/ai_behaviour_props.h"
#include "resources/weapon.h"

#include "components/ai/vision_component.h"
#include "components/ai/detection_area_component.h"
#include "components/ai/ai_character_component.h"

#include "components/weapon_effects_components.h"
#include "components/input_component.h"
#include "components/muzzle_flash_component.h"
#include "components/weapon_wrapper.h"
#include "components/ai/vision_component.h"

// Godot Environment Query System Headers
#include "geqo/query_enums.h"
#include "geqo/query_instance.h"
#include "geqo/query_item.h"
#include "geqo/contexts/query_context3d.h"
#include "geqo/generators/query_generator3d.h"
#include "geqo/query_result.h"
#include "geqo/tests/query_test3d.h"
#include "geqo/debug/geqo_debug.h"
#include "geqo/debug/geqo_debug_spheres3d.h"
#include "geqo/environment_query3d.h"

// Contexts
#include "geqo/contexts/context_querier3d.h"
#include "geqo/contexts/context_in_group3d.h"
#include "geqo/contexts/context_target_array3d.h"
#include "geqo/contexts/context_target_node3d.h"
#include "geqo/contexts/context_query_items3d.h"

// Generators
#include "geqo/generators/generator_circle_shape3d.h"
#include "geqo/generators/generator_grid_shape3d.h"
#include "geqo/generators/generator_in_array3d.h"
#include "geqo/generators/generator_in_group3d.h"
#include "geqo/generators/generator_composite3d.h"

// Tests
#include "geqo/tests/test_distance_to3d.h"
#include "geqo/tests/test_dot_product3d.h"
#include "geqo/tests/test_raycast_to3d.h"
#include "geqo/tests/test_path_find_to3d.h"
#include "geqo/tests/test_intersects_area3d.h"

using namespace godot;

void initailize(ModuleInitializationLevel p_level);
void uinitailize(ModuleInitializationLevel p_leevel);
