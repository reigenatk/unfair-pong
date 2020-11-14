#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "game_window.h"

namespace unfairpong {

namespace visualizer {
/**
 * Runs the animation for ideal gas
 */
class UnfairPongApp : public ci::app::App {
 public:

  UnfairPongApp();


  void setup() override;
  void update() override;
  void draw() override;

  // allows user to speed up or slow down the simulation
  // down key is slow down, up key is speed up
  void keyDown(ci::app::KeyEvent event) override;

 private:
  GameWindow unfair_pong_window;



};

}  // namespace visualizer

}  // namespace idealgas
