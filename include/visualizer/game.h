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

    size_t points_to_win_ = 10;

    double starting_velocity_cap_ = 5;

    Game();

    Game(vec2 top_left, double length, double height);

    void Draw() ;

    static double Game::GenerateRandomDouble(double absolute_value_limit);

    void HandleMovement(const vec2& mouse_coords);

    void UpdateBall();

    void UpdateUserBumper();

    void UpdateCpuBumper();

    void CheckIfPlayerScored();

    void ExecuteBallWallCollision();

    void ExecuteBallUserBumperCollision();

    void ExecuteBallCpuBumperCollision();

    vec2 RandomVelocityGivenSpeed(double speed_desired, bool positive_y_velocity);

    void GameReset();

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
    bool is_round_running_;
    size_t user_score_;
    size_t cpu_score_;


    Ball ball_in_play;
    CpuBumper cpu_bumper_;
    UserBumper user_bumper_;
};

}  // namespace visualizer

}  // namespace naivebayes
