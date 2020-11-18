
#include <core/user_bumper.h>
#include <iostream>
#include <cinder/gl/gl.h>

using std::cout;

namespace unfairpong {

    UserBumper::UserBumper() {

    }

    UserBumper::UserBumper(vec2 center_position, double length_of_bumper, cinder::Color color,
                           double thickness, float left_wall, float right_wall) {
        center_position_ = center_position;
        length_of_bumper_ = length_of_bumper;
        bumper_color_ = color;
        thickness_of_bumper_ = thickness;
        horizontal_velocity_of_bumper = 0;
        left_wall_ = left_wall;
        right_wall_ = right_wall;

        // this value is basically how sensitive bumper is to left or right keys
        // not sure which one is best so I just left it here
        bumper_sensitivity_ = 3.0;
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

    void UserBumper::Draw() const {
        ci::gl::color(bumper_color_);
        vec2 top_left_corner_ = center_position_ + vec2(-length_of_bumper_ / 2.0, -thickness_of_bumper_);
        vec2 bottom_right_corner = center_position_ + vec2(length_of_bumper_ / 2.0, 0);
        ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
        ci::gl::drawSolidRect(pixel_bounding_box);
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

    void UserBumper::MoveBumperLeft() {
        horizontal_velocity_of_bumper -= bumper_sensitivity_;
    }

    void UserBumper::MoveBumperRight() {
        horizontal_velocity_of_bumper += bumper_sensitivity_;
    }

}