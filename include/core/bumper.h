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

    void ResetForNewRound(const vec2& new_position);
    void ExecuteBrittleCollision(const vec2& ball_coords);
    /**
     * This method "decides" what type of ball the next one should be and is called upon each collision
     * of ball and bumper. Implemented differently between user and CPU bumper since user can only spawn smash
     * while CPU can spawn all 5 kinds of balls
     * @return
     */
    virtual BallType GenerateBallType() = 0;

    /**
     * Draws the bumper in a certain way depending on if it is the top bumper
     * @param is_top_bumper
     */
    void Draw(bool is_top_bumper) const;

    /**
     * Two abstract methods meant for CPU and User movement, respectively. Implemented in both bumpers but
     * only have actual functionality for one of them
     */
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
    double original_length_of_bumper; // need this field for brittle balls- since when we reset the round we need to know what old length was
    cinder::Color bumper_color_;
    double thickness_of_bumper_;
    float left_wall_;
    float right_wall_;

};

}

#endif //UNFAIR_PONG_BUMPER_H
