
#ifndef UNFAIR_PONG_USER_BUMPER_H
#define UNFAIR_PONG_USER_BUMPER_H

#include <cinder/Color.h>
#include <core/bumper.h>
#include "../../../../include/glm/glm.hpp"
using glm::vec2;

namespace unfairpong {

class UserBumper: public Bumper {
public:
    UserBumper();
    UserBumper(vec2 center_position, double length_of_bumper, cinder::Color color,
               double thickness, float left_wall, float right_wall);


    /**
     * This method is used when being controlled with arrow keys- if the bumper has a velocity
     * then we need to update its new position. Method is called in the update() function
     * Using mouse is a different story though because we don't use velocity in mouse controls
     */
    void ExecuteTimeStep();
    /**
     * These two methods are called when keyevents are detected in the unfair_pong_app
     * They add velocity to the bumper horizontally in both directions
     */
    void MoveBumperLeft();
    void MoveBumperRight();


    /**
     * Use this method when user is moving bumper with mouse- we need to place a limit on
     * where the bumper can be though so there are special cases, not just a simple setter
     */
    void SteerBumperWithMouse(const vec2 &mouse_coords);

    void Draw() const;

private:
    // this member variable is unique to user_bumper, essentially the way I implemented left/right arrow key movement
    // is that it adds a velocity left or right, and that it deccelerates at a constant value over time
    // and we decide where it travels using this member variable
    float horizontal_velocity_of_bumper;

    float bumper_sensitivity_;

};

}


#endif //UNFAIR_PONG_USER_BUMPER_H
