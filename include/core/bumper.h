#ifndef UNFAIR_PONG_BUMPER_H
#define UNFAIR_PONG_BUMPER_H

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "../../../../include/glm/glm.hpp"
#include "balltype.h"

using glm::vec2;

namespace unfairpong {

class Bumper {

public:
    Bumper();
    Bumper(vec2 center_position, double length_of_bumper, cinder::Color color,
           double thickness, float left_wall, float right_wall);

    double GetBumperThickness() const;
    vec2& GetBumperCenter();
    double GetBumperLength() const;

    // get rid of setters and replace with brittle collision method

    void ResetForNewRound(const vec2& new_position);
    void ExecuteBrittleCollision(const vec2& ball_coords);
    virtual BallType GenerateBallType() = 0;
    void Draw(bool is_top_bumper) const;

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
