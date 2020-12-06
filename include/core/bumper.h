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

    // this is a method that only the CPU bumper will add to since the user bumper
    // does not get controlled by anything but mouse
    virtual void SmartMovement(vec2 ball_position, vec2 ball_velocity) = 0;
    virtual void MouseMovement(vec2 mouse_coords) = 0;
    /**
     * Returns the vec2 coords of the corner that is further away from center of bumper
     * so either (leftwall.x, bottomwall.y) or (rightwall.x, bottomwall.y)
     * @return
     */
    vec2 FartherCorner() const;

protected:
    vec2 center_position_;
    double length_of_bumper_;
    double original_length_of_bumper; // need this field for brittle balls- else bumper forgets what its old length was with a new round
    cinder::Color bumper_color_;
    double thickness_of_bumper_;
    float left_wall_;
    float right_wall_;

};

}

#endif //UNFAIR_PONG_BUMPER_H
