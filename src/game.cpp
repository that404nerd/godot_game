#include "game.h"

Game::Game() {
}

void Game::_ready()
{
}

void Game::_bind_methods() {}

void Game::_unhandled_input(const Ref<InputEvent>& event)
{
    if (Input::get_singleton()->is_action_just_pressed("escape_focus")) {
        if (Input::get_singleton()->get_mouse_mode() == Input::MOUSE_MODE_CAPTURED) {
            Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
        } else {
            Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
        }
    }

}

void Game::_process(double delta) 
{
    Engine::get_singleton()->set_max_fps(144);
}

Game::~Game() {}