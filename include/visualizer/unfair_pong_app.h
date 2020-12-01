#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "game.h"

namespace unfairpong {

namespace visualizer {
/**
 * Runs the animation for ideal gas
 */
class UnfairPongApp : public ci::app::App {
 public:

  const double window_length_ = 800;
  const double window_height_ = 800;

  UnfairPongApp();

  void setup() override;
  void update() override;
  void draw() override;

  // allows user to speed up or slow down the simulation
  // down key is slow down, up key is speed up
  void keyDown(ci::app::KeyEvent event) override;

  void mouseDown(ci::app::MouseEvent event);
  void mouseDrag(ci::app::MouseEvent event);

 private:
  Game unfair_pong_instance;
  cinder::gl::Texture2dRef sad_emoji_;


};

}  // namespace visualizer

}  // namespace idealgas
