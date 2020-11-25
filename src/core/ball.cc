#include <visualizer/game.h>
#include <core/ball.h>
#include <iostream>

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
}

void Ball::CollideWithUserBumper() {


    if (Game::RollChance(user_smash_rate_)) {
        // user gets smash
        velocity_ = vec2(0, -length(velocity_) - 2);
        ChangeIntoRandomColor();
    }
    else {
        velocity_ = Game::RandomVelocityGivenSpeed(length(velocity_) + difficulty_increment_,
                                                         false);
    }

}

void Ball::CollideWithCpuBumper() {

    if (Game::RollChance(cpu_smash_rate_)) {
        // cpu gets smash
        velocity_ = vec2(0, length(velocity_) + 2);
        ChangeIntoRandomColor();
    }
    else {
        velocity_ = Game::RandomVelocityGivenSpeed(length(velocity_) + difficulty_increment_,
                                                   true);
    }
}

void Ball::Draw() const {
    ci::gl::color(color_);
    ci::gl::drawSolidCircle(position_, (float) radius_);
}

void Ball::UpdatePositionWithVelocity() {
    position_ += velocity_;
}

}