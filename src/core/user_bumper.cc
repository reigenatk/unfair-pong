
#include <core/user_bumper.h>
#include <iostream>
#include <cinder/gl/gl.h>

using std::cout;

namespace unfairpong {

    UserBumper::UserBumper() {

    }

    UserBumper::UserBumper(vec2 center_position, double length_of_bumper, cinder::Color color,
                           double thickness, float left_boundary, float right_boundary) {
        center_position_ = center_position;
        length_of_bumper_ = length_of_bumper;
        bumper_color_ = color;
        thickness_of_bumper_ = thickness;
        horizontal_velocity_of_bumper = 0;
        left_boundary_ = left_boundary;
        right_boundary_ = right_boundary;
    }

    double UserBumper::GetBumperThickness() {
        return thickness_of_bumper_;
    }

    vec2& UserBumper::GetBumperCenter() {
        return center_position_;
    }

    double UserBumper::GetBumperLength() {
        return length_of_bumper_;
    }

    void UserBumper::Draw() const {
        ci::gl::color(bumper_color_);
        vec2 top_left_corner_ = center_position_ + vec2(-length_of_bumper_ / 2.0, -thickness_of_bumper_);
        vec2 bottom_right_corner = center_position_ + vec2(length_of_bumper_ / 2.0, 0);
        ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
        ci::gl::drawSolidRect(pixel_bounding_box);

        ci::gl::drawStringCentered("bumper velocity: " + std::to_string(horizontal_velocity_of_bumper),
                                  vec2(100, 10),
                                  ci::Color("black"), ci::Font("Helvetica", 15));
    }

    void UserBumper::ExecuteTimeStep() {
        if (center_position_.x - (length_of_bumper_ / 2.0) + horizontal_velocity_of_bumper < left_boundary_) {
            center_position_.x = left_boundary_ + (float) (length_of_bumper_ / 2.0);
            horizontal_velocity_of_bumper = 0;
        }
        else if (center_position_.x + (length_of_bumper_ / 2.0) + horizontal_velocity_of_bumper > right_boundary_) {
            center_position_.x = right_boundary_ - (float) (length_of_bumper_ / 2.0);
            horizontal_velocity_of_bumper = 0;
        }
        else {
            center_position_.x += horizontal_velocity_of_bumper;
        }
        if (abs(horizontal_velocity_of_bumper) != 0) {
            if (horizontal_velocity_of_bumper < 0) {
                horizontal_velocity_of_bumper = fmin(0.0f, horizontal_velocity_of_bumper + 0.05f);
            }
            else if (horizontal_velocity_of_bumper > 0) {
                horizontal_velocity_of_bumper = fmax(0.0f, horizontal_velocity_of_bumper - 0.05f);
            }
        }
    }

    void UserBumper::MoveBumperLeft() {
        horizontal_velocity_of_bumper -= 3.0;
    }

    void UserBumper::MoveBumperRight() {
        horizontal_velocity_of_bumper += 3.0;
    }

}