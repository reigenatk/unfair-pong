
#include <core/cpu_bumper.h>
#include <iostream>

using std::cout;

namespace unfairpong {

    CpuBumper::CpuBumper() {

    }

    CpuBumper::CpuBumper(vec2 center_position, double length_of_bumper, cinder::Color color, double thickness) {
        center_position_ = center_position;
        length_of_bumper_ = length_of_bumper;
        bumper_color_ = color;
        thickness_of_bumper_ = thickness;
    }

    void CpuBumper::Draw() const {
        ci::gl::color(bumper_color_);
        vec2 top_left_corner_ = center_position_ + vec2(-length_of_bumper_ / 2.0, 0);
        vec2 bottom_right_corner = center_position_ + vec2(length_of_bumper_ / 2.0, thickness_of_bumper_);
        ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
        ci::gl::drawSolidRect(pixel_bounding_box);
    }

}