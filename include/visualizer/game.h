#pragma once

#include <core/ball.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <core/cpu_bumper.h>
#include <core/user_bumper.h>


using std::pair;
using std::vector;

namespace unfairpong {

namespace visualizer {


class Game {
 public:

    Game();

    Game(vec2 top_left, double length, double height);

    void Draw() const;

    void UpdateBall();

    void UpdateUserBumper();

    void UpdateCpuBumper();

    void keyDown(ci::app::KeyEvent event);

    UserBumper& GetUserBumper();

 private:
    vec2 top_left_corner_;
    double length_;
    double height_;

    double left_wall_;
    double top_wall_;
    double bottom_wall_;
    double right_wall_;


    bool is_game_running_;
    size_t user_score_;
    size_t cpu_score_;
    size_t points_to_win_;

    Ball ball_in_play;
    CpuBumper cpu_bumper_;
    UserBumper user_bumper_;
};

}  // namespace visualizer

}  // namespace naivebayes
