#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/physics_server3d.hpp>

#include "player_state_machine.h"
#include "player.h"

class PlayerSlideState : public State {

public:
  PlayerSlideState(PlayerStateMachine* playerStateMachine) 
    : State(static_cast<int8_t>(PlayerStates::SLIDE)),
        m_PlayerStateMachine(playerStateMachine)
  {}; 

  virtual void _enter() override;
  virtual void _handle_input(const Ref<InputEvent>& event) override;
  virtual void _physics_update(double delta) override;

  void _crouch_player();
  void _on_slide_finished();

  virtual void _exit() override;

private:
  float m_FinalPos { 0.0f }, m_SlideTimer { 0.0f };
  Vector3 m_OriginalHeadPosition { Vector3(0.0f, 0.0f, 0.0f) };
  Player* m_PlayerInst { nullptr };
  PlayerStateMachine* m_PlayerStateMachine { nullptr };

  Vector3 m_SlideVector { Vector3(0.0f, 0.0f, 0.0f) };

  Ref<Tween> m_CrouchTween;
};
