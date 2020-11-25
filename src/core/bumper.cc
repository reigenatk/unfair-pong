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
double Bumper::GetBumperLength() {
    return length_of_bumper_;
}

double Bumper::GetBumperThickness() {
    return thickness_of_bumper_;
}

void Bumper::ResetForNewRound(const vec2 &new_position) {
    center_position_ = new_position;
}


}
