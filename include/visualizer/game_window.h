#pragma once

#include <core/ball.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"


using std::pair;
using std::vector;

namespace unfairpong {

namespace visualizer {


class GameWindow {
 public:

 private:

  glm::vec2 top_left_corner_;

  double length_of_box_;

  double height_of_box_;

  // y_bound is the amount of vertical space we have used so far to spawn particles in. This is to
  // make sure we don't spawn particles on top of one another
  double y_bound;

  vector<Particle> particles_;
};

}  // namespace visualizer

}  // namespace naivebayes
