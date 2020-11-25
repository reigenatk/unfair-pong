#ifndef UNFAIR_PONG_BUMPER_H
#define UNFAIR_PONG_BUMPER_H

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "../../../../include/glm/glm.hpp"
using glm::vec2;


namespace unfairpong {

class Bumper {

public:
    Bumper();
    Bumper(vec2 center_position, double length_of_bumper, cinder::Color color,
           double thickness, float left_wall, float right_wall);

    double GetBumperThickness();
    vec2& GetBumperCenter();
    double GetBumperLength();
    void ResetForNewRound(const vec2& new_position);


protected:
    vec2 center_position_;
    double length_of_bumper_;
    cinder::Color bumper_color_;
    double thickness_of_bumper_;
    float left_wall_;
    float right_wall_;

};

}

#endif //UNFAIR_PONG_BUMPER_H
