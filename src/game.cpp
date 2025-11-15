#include "game.h"

Game::Game() {
}

void Game::_ready()
{
    m_FPSLabel = get_node<Label>(NodePath("TextDebugLayer/DebugInfoLayer"));
}

void Game::_bind_methods() {}

void Game::_process(double delta) 
{
    Engine::get_singleton()->set_max_fps(144);

    String fps_text = "FPS: " + String::num(Engine::get_singleton()->get_frames_per_second());
    String state_text = "Current State: " + String(FStateManager::GetStateManagerInstance().get_current_player_state().c_str());
    m_FPSLabel->set_text(fps_text + "\n" + state_text);
}

Game::~Game() {}