#include "game.h"

Game::Game() {
}

void Game::_ready()
{
    m_FPSLabel = get_node<Label>("TextDebugLayer/FPSLabel");
}

void Game::_bind_methods() {}

void Game::_process(double delta) 
{
    Engine::get_singleton()->set_max_fps(144);
    m_FPSLabel->set_text("FPS: " + String::num(Engine::get_singleton()->get_frames_per_second()));
}

Game::~Game() {}