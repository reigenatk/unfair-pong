#include <core/bumper.h>

namespace unfairpong {

Bumper::Bumper() {

}

Bumper::Bumper(vec2 center_position, double length_of_bumper, cinder::Color color, double thickness,
               float left_wall, float right_wall) {
    center_position_ = center_position;
    length_of_bumper_ = length_of_bumper;
    bumper_color_ = color;
    thickness_of_bumper_ = thickness;
    left_wall_ = left_wall;
    right_wall_ = right_wall;
}

vec2 & Bumper::GetBumperCenter() {
    return center_position_;
}
double Bumper::GetBumperLength() const {
    return length_of_bumper_;
}

double Bumper::GetBumperThickness() const {
    return thickness_of_bumper_;
}

void Bumper::ExecuteBrittleCollision(const vec2 &ball_coords) {
    if (ball_coords.x >= center_position_.x) {
        // if the ball landed on the right side of the bumper

        vec2 new_bumper_left = center_position_ + vec2(-length_of_bumper_ / 2.0, 0);
        vec2 new_bumper_right = vec2(ball_coords.x, center_position_.y);
        double amount_to_break = center_position_.x + (length_of_bumper_ / 2.0) - ball_coords.x;
        length_of_bumper_ = length_of_bumper_ - amount_to_break;

        vec2 new_center = new_bumper_left + new_bumper_right;
        new_center.x /= 2.0;
        new_center.y /= 2.0;
        center_position_ = new_center;
    }
    else {
        // if the ball landed on the left side of the bumper
        vec2 new_bumper_left = vec2(ball_coords.x, center_position_.y);
        vec2 new_bumper_right = center_position_ + vec2(length_of_bumper_ / 2.0, 0);
        double amount_to_break = ball_coords.x - (center_position_.x - (length_of_bumper_ / 2.0));
        length_of_bumper_ = length_of_bumper_ - amount_to_break;

        vec2 new_center = new_bumper_left + new_bumper_right;
        new_center.x /= 2.0;
        new_center.y /= 2.0;
        center_position_ = new_center;
    }
}

void Bumper::Draw(bool is_top_bumper) const {
    if (is_top_bumper) {
        ci::gl::color(bumper_color_);
        vec2 top_left_corner_ = center_position_ + vec2(-length_of_bumper_ / 2.0, 0);
        vec2 bottom_right_corner = center_position_ + vec2(length_of_bumper_ / 2.0, thickness_of_bumper_);
        ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
        ci::gl::drawSolidRect(pixel_bounding_box);
    }
    else {
        ci::gl::color(bumper_color_);
        vec2 top_left_corner_ = center_position_ + vec2(-length_of_bumper_ / 2.0, -thickness_of_bumper_);
        vec2 bottom_right_corner = center_position_ + vec2(length_of_bumper_ / 2.0, 0);
        ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
        ci::gl::drawSolidRect(pixel_bounding_box);
    }
}

vec2 Bumper::FartherCorner() const {
    if (center_position_.x - left_wall_ <= right_wall_ - center_position_.x) {
        return vec2(right_wall_, center_position_.y);
    }
    else {
        return vec2(left_wall_, center_position_.y);
    }
}

void Bumper::ResetForNewRound(const vec2 &new_position) {
    center_position_ = new_position;

}


}
