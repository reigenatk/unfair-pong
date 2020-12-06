
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
    horizontal_velocity_of_bumper = 0;
    user_smash_rate_ = user_smash_rate;

    // this value is basically how sensitive bumper is to left or right arrow keys
    // not sure which value is best so I just left it here
    bumper_sensitivity_ = 3.0;
}



void UserBumper::SteerBumperWithMouse(const vec2 &mouse_coords) {
    // also do a reset on the velocity here- otherwise bumper will keep moving
    // after clicking down
    horizontal_velocity_of_bumper = 0;
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

void UserBumper::ExecuteTimeStep() {
    if (center_position_.x - (length_of_bumper_ / 2.0) + horizontal_velocity_of_bumper < left_wall_) {
        center_position_.x = left_wall_ + (float) (length_of_bumper_ / 2.0);
        horizontal_velocity_of_bumper = 0;
    }
    else if (center_position_.x + (length_of_bumper_ / 2.0) + horizontal_velocity_of_bumper > right_wall_) {
        center_position_.x = right_wall_ - (float) (length_of_bumper_ / 2.0);
        horizontal_velocity_of_bumper = 0;
    }
    else {
        center_position_.x += horizontal_velocity_of_bumper;
    }

    // I slowly decrease the velocity over time so that if the user stops holding the left or right keys
    // then bumper eventually stops
    float velocity_decay = 0.05f;

    if (abs(horizontal_velocity_of_bumper) != 0) {
        if (horizontal_velocity_of_bumper < 0) {
            horizontal_velocity_of_bumper = fmin(0.0f, horizontal_velocity_of_bumper + velocity_decay);
        }
        else if (horizontal_velocity_of_bumper > 0) {
            horizontal_velocity_of_bumper = fmax(0.0f, horizontal_velocity_of_bumper - velocity_decay);
        }
    }
}

vec2 UserBumper::FartherCorner() const {
    if (center_position_.x - left_wall_ <= right_wall_ - center_position_.x) {
        return vec2(right_wall_, center_position_.y);
    }
    else {
        return vec2(left_wall_, center_position_.y);
    }
}

void UserBumper::MoveBumperLeft() {
    horizontal_velocity_of_bumper -= bumper_sensitivity_;
}

void UserBumper::MoveBumperRight() {
    horizontal_velocity_of_bumper += bumper_sensitivity_;
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