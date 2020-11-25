
#include <core/cpu_bumper.h>
#include <iostream>

using std::cout;

namespace unfairpong {

    CpuBumper::CpuBumper() {

    }

    CpuBumper::CpuBumper(vec2 center_position, double length_of_bumper, cinder::Color color, double thickness,
                         float max_movement_speed, float left_wall, float right_wall) {
        center_position_ = center_position;
        length_of_bumper_ = length_of_bumper;
        bumper_color_ = color;
        thickness_of_bumper_ = thickness;
        max_movement_speed_ = max_movement_speed;
        left_wall_ = left_wall;
        right_wall_ = right_wall;
    }

    double CpuBumper::GetBumperThickness() {
        return thickness_of_bumper_;
    }

    vec2& CpuBumper::GetBumperCenter() {
        return center_position_;
    }

    void CpuBumper::ResetForNewRound(const vec2 &new_position) {
        center_position_ = new_position;
    }

    double CpuBumper::GetBumperLength() {
        return length_of_bumper_;
    }

    void CpuBumper::MakeMovementDecision(const vec2& ball_position, const vec2& ball_velocity) {
        // if the ball is not within length_of_bumper / 2 of the walls, we can attempt to move the
        // center of the bumper to where the ball is. "tracking the ball in a sense"

        double half_length = length_of_bumper_ / 2.0;

        if (ball_position.x >= left_wall_ + (float) half_length && ball_position.x <= right_wall_ - (float) half_length) {
            if (center_position_.x < ball_position.x) {
                float gap = ball_position.x - center_position_.x;
                center_position_.x += fmin(max_movement_speed_, gap);
            }
            else if (center_position_.x > ball_position.x) {
                float gap = center_position_.x - ball_position.x;
                center_position_.x -= fmin(max_movement_speed_, gap);
            }
        }

        // otherwise we will move the bumper as close as possible to the wall towards a designated location
        // since the center of the bumper can only get so close to the wall
        // before one of its ends runs over the right or left walls
        if (ball_position.x < left_wall_ + (float) half_length) {
            float gap = center_position_.x - (left_wall_ + (float) half_length);
            center_position_.x -= fmin(max_movement_speed_, gap);
        }

        if (ball_position.x > right_wall_ - (float) half_length) {
            float gap = (right_wall_ - (float) half_length) - center_position_.x;
            center_position_.x += fmin(max_movement_speed_, gap);
        }
    }

    void CpuBumper::Draw() const {
        ci::gl::color(bumper_color_);
        vec2 top_left_corner_ = center_position_ + vec2(-length_of_bumper_ / 2.0, 0);
        vec2 bottom_right_corner = center_position_ + vec2(length_of_bumper_ / 2.0, thickness_of_bumper_);
        ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
        ci::gl::drawSolidRect(pixel_bounding_box);
    }

}