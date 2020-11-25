#include <visualizer/game.h>
#include <core/ball.h>
#include <iostream>
#include <algorithm>

using std::cout;
using unfairpong::visualizer::Game;

namespace unfairpong {

Ball::Ball() {

}

Ball::Ball(vec2 starting_position, vec2 starting_velocity, cinder::Color color,
           double radius, double user_smash_rate, double cpu_smash_rate, double difficulty_increment) {
    position_ = starting_position;
    velocity_ = starting_velocity;
    color_ = color;
    radius_ = radius;
    user_smash_rate_ = user_smash_rate;
    cpu_smash_rate_ = cpu_smash_rate;
    difficulty_increment_ = difficulty_increment;

    max_ball_velocity = 20; // can tweak this but I found 20 to work ok
    smash_velocity_increase = 10; // just how much faster does the ball get on a smash hit
    is_smash_ball = false;
}

vec2& Ball::GetPosition() {
    return position_;
}

vec2& Ball::GetVelocity() {
    return velocity_;
}

double Ball::GetRadius() {
    return radius_;
}

cinder::Color Ball::GetColor() {
    return color_;
}

void Ball::ChangeIntoRandomColor() {
    double r = Game::GenerateRandomDoubleBetween(0, 1);
    double g = Game::GenerateRandomDoubleBetween(0, 1);
    double b = Game::GenerateRandomDoubleBetween(0, 1);

    color_ = cinder::Color((float) r, (float) g, (float) b);
}

void Ball::ResetForNewRound(vec2 new_position, vec2 new_velocity) {
    position_ = new_position;
    velocity_ = new_velocity;
    is_smash_ball = false;
}

vec2 Ball::VelocityGivenTargetAndSpeed(vec2 target_pos, double desired_velocity_of_ball) {
    double xdiff = target_pos.x - position_.x;
    double ydiff = target_pos.y - position_.y;

    vec2 ret = vec2(xdiff, ydiff);
    double len = length(ret);
    double scale_factor = desired_velocity_of_ball / len;
    ret.x *= (float) scale_factor;
    ret.y *= (float) scale_factor;

    return ret;
}

void Ball::CollideWithUserBumper(vec2 center_of_user_bumper, float left_wall_x, float right_wall_x, float top_wall_y) {


    if (Game::RollChance(user_smash_rate_)) {
        // user gets smash

        double target_x_location = Game::GenerateRandomDoubleBetween(left_wall_x, right_wall_x);
        double velocity_of_ball = fmin(max_ball_velocity, length(velocity_) + smash_velocity_increase);
        velocity_ = VelocityGivenTargetAndSpeed(vec2(target_x_location, top_wall_y), velocity_of_ball);
        is_smash_ball = true;
    }
    else {
        velocity_ = Game::RandomVelocityGivenSpeed(fmin(max_ball_velocity, length(velocity_) + difficulty_increment_),
                                                         false);
        is_smash_ball = false;
    }

}

void Ball::CollideWithCpuBumper(vec2 center_of_cpu_bumper, float left_wall_x, float right_wall_x, float bottom_wall_y) {

    if (Game::RollChance(cpu_smash_rate_)) {
        // cpu gets smash
        double target_x_location = Game::GenerateRandomDoubleBetween(left_wall_x, right_wall_x);
        double velocity_of_ball = fmin(max_ball_velocity, length(velocity_) + smash_velocity_increase);
        velocity_ = VelocityGivenTargetAndSpeed(vec2(target_x_location, bottom_wall_y), velocity_of_ball);
        is_smash_ball = true;
    }
    else {
        velocity_ = Game::RandomVelocityGivenSpeed(fmin(max_ball_velocity, length(velocity_) + difficulty_increment_),
                                                   true);
        is_smash_ball = false;
    }
}

void Ball::Draw() const {
    if (is_smash_ball) {
        ci::gl::color(ci::Color("white"));
        cinder::gl::Texture2dRef texture = cinder::gl::Texture2d::create
                (cinder::loadImage("../../../data/smashball.png"));
        ci::gl::draw(texture, position_);
    }
    ci::gl::color(color_);
    ci::gl::drawSolidCircle(position_, (float) radius_);
}

void Ball::UpdatePositionWithVelocity() {
    position_ += velocity_;
}

}