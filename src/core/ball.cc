
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

    void Ball::Draw() const {
        ci::gl::color(color_);
        ci::gl::drawSolidCircle(position_, (float) radius_);
    }

    void Ball::UpdatePositionWithVelocity() {
        position_ += velocity_;
    }

}