#pragma once

#include <godot_cpp/core/class_db.hpp>

#include "game.h"
#include "camera_controller.h"
#include "player.h"
#include "player_state.h"
#include "player_state_machine.h"
#include "player_idle_state.h"
#include "player_sprint_state.h"
#include "player_dash_state.h"
#include "player_jump_state.h"
#include "player_falling_state.h"
#include "player_crouch_state.h"
#include "player_slide_state.h"
#include "weapon.h"
#include "weapon_manager.h"
#include "weapon_state.h"
#include "global_state_handler.h"

#include "components/ammo_component.h"
#include "components/weapon_effects_components.h"
#include "components/weapon_component.h"
#include "components/character_component.h"

#include "game_manager.h"

using namespace godot;

void initailize(ModuleInitializationLevel p_level);
void uinitailize(ModuleInitializationLevel p_leevel);
