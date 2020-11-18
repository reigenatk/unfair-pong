
#ifndef UNFAIR_PONG_USER_BUMPER_H
#define UNFAIR_PONG_USER_BUMPER_H

#include <cinder/Color.h>
#include "../../../../include/glm/glm.hpp"
using glm::vec2;

namespace unfairpong {

    class UserBumper {
    public:
        UserBumper();
        UserBumper(vec2 center_position, double length_of_bumper, cinder::Color color,
                   double thickness, float left_wall, float right_wall);
        void Draw() const;

        /**
         * This method is used when being controlled with arrow keys- if the bumper has a velocity
         * then we need to update its new position. Method is called in the update() function
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

        double GetBumperThickness();
        vec2& GetBumperCenter();
        double GetBumperLength();

    private:
        // this member variable is unique to user_bumper, essentially the way I implemented left/right arrow key movement
        // is that it adds a velocity left or right, and that it deccelerates at a constant value over time
        // and we decide where it travels using this member variable
        float horizontal_velocity_of_bumper;

        // these member variables are common between user and cpu bumper
        vec2 center_position_;
        double length_of_bumper_;
        cinder::Color bumper_color_;
        double thickness_of_bumper_;
        float left_wall_;
        float right_wall_;
    };

}


#endif //UNFAIR_PONG_USER_BUMPER_H
