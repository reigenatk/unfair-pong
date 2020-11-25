#include <visualizer/game.h>
#include <core/ball.h>
#include <iostream>

using std::cout;

namespace unfairpong {

    Ball::Ball() {

    }

    Ball::Ball(vec2 starting_position, vec2 starting_velocity, cinder::Color color, double radius) {
        position_ = starting_position;
        velocity_ = starting_velocity;
        color_ = color;
        radius_ = radius;
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

    void Ball::ResetForNewRound(vec2 new_position, vec2 new_velocity) {
        position_ = new_position;
        velocity_ = new_velocity;
    }


    void Ball::Draw() const {
        ci::gl::color(color_);
        ci::gl::drawSolidCircle(position_, (float) radius_);
    }

    void Ball::UpdatePositionWithVelocity() {
        position_ += velocity_;
    }

}