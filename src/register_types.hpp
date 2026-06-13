#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "game.h"
#include "camera_controller.h"
#include "player.h"
#include "weapon.h"

#include "state_machine.h"
#include "player_state_machine.h"
#include "weapon_state_machine.h"
#include "weapon_manager.h"
#include "camera_recoil_holder.h"
#include "event_bus.h"

#include "components/muzzle_flash_component.h"
#include "components/weapon_wrapper.h"

using namespace godot;

void initailize(ModuleInitializationLevel p_level);
void uinitailize(ModuleInitializationLevel p_leevel);
