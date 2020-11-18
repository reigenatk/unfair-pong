#ifndef UNFAIR_PONG_CPU_BUMPER_H
#define UNFAIR_PONG_CPU_BUMPER_H

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "../../../../include/glm/glm.hpp"
using glm::vec2;

namespace unfairpong {
    class CpuBumper {
    public:
        CpuBumper();

        /**
         * Parameters are the same as in user_bumper constructor except for max_movement_speed
         * which is a bound on how fast the cpu bumper can travel (so that user can beat cpu)
         */
        CpuBumper(vec2 center_position, double length_of_bumper, cinder::Color color, double thickness,
                  float max_movement_speed, float left_wall, float right_wall);
        void Draw() const;

        double GetBumperThickness();
        vec2& GetBumperCenter();
        double GetBumperLength();

        /**
         * As opposed to user bumper where user input dictates movement, this method is called for cpu bumper
         * The way it decides where to move is it compares the position of its center (in center_position member variable)
         * and tries to match it with the cetner position of the ball. If the ball is too close to the walls
         * it just waits next to that wall
         */
        void MakeMovementDecision(const vec2& ball_position, const vec2& ball_velocity);

    private:
        // fastest speed at which the bumper may travel
        float max_movement_speed_;

        // these member variables are common between user and cpu bumper
        vec2 center_position_;
        double length_of_bumper_;
        cinder::Color bumper_color_;
        double thickness_of_bumper_;
        float left_wall_;
        float right_wall_;

    };

}








#endif //UNFAIR_PONG_CPU_BUMPER_H
