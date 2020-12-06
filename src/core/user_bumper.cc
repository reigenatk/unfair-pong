#include <core/bumper.h>
#include <core/user_bumper.h>
#include <iostream>
#include <cinder/gl/gl.h>
#include <visualizer/game.h>

using unfairpong::visualizer::Game;

using std::cout;

namespace unfairpong {

UserBumper::UserBumper() {

}

UserBumper::UserBumper(vec2 center_position, double length_of_bumper, cinder::Color color,
                       double thickness, float left_wall, float right_wall, double user_smash_rate):
                       Bumper(center_position, length_of_bumper, color, thickness, left_wall, right_wall) {
    user_smash_rate_ = user_smash_rate;

    // this value is basically how sensitive bumper is to left or right arrow keys
    // not sure which value is best so I just left it here
}



void UserBumper::SteerBumperWithMouse(const vec2 &mouse_coords) {

    if (mouse_coords.x < left_wall_ + (float) (length_of_bumper_ / 2.0)) {
        center_position_.x = left_wall_ + (float) (length_of_bumper_ / 2.0);
    }
    else if (mouse_coords.x > right_wall_ - (float) (length_of_bumper_ / 2.0)) {
        center_position_.x = right_wall_ - (float) (length_of_bumper_ / 2.0);
    }
    else {
        center_position_.x = mouse_coords.x;
    }
}

void UserBumper::MouseMovement(vec2 mouse_coords) {
    SteerBumperWithMouse(mouse_coords);
}

void UserBumper::SmartMovement(vec2 ball_pos, vec2 ball_vel) {
    // do nothing lol
}

BallType UserBumper::GenerateBallType() {
    if (Game::RollChance(user_smash_rate_)) {
        return Smash;
    }
    else {
        return Normal;
    }
}

}