#ifndef UNFAIR_PONG_CPU_BUMPER_H
#define UNFAIR_PONG_CPU_BUMPER_H

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "../../../../include/glm/glm.hpp"
#include "bumper.h"

using glm::vec2;

namespace unfairpong {

class CpuBumper: public Bumper {

public:
    CpuBumper();

    /**
     * Parameters are the same as in user_bumper constructor except for max_movement_speed
     * which is a bound on how fast the cpu bumper can travel (so that user can beat cpu)
     * We also include information about rates for each kind of ball to spawn
     */
    CpuBumper(vec2 center_position, double length_of_bumper, cinder::Color color, double thickness,
              float max_movement_speed, float left_wall, float right_wall, double cpu_smash_rate,
              double cpu_dizzy_rate, double cpu_monkey_rate, double cpu_brittle_rate, double cpu_random_rate);


    /**
     * As opposed to user bumper where user input dictates movement, this method is called for cpu bumper
     * The way it decides where to move is it compares the position of its center (in center_position member variable)
     * and tries to match it with the cetner position of the ball. If the ball is too close to the walls
     * it just waits next to that wall
     */
    void MakeMovementDecision(const vec2& ball_position, const vec2& ball_velocity);

    BallType GenerateBallType() override;

    void SmartMovement(vec2 ball_pos, vec2 ball_vel) override;

    void MouseMovement(vec2 mouse_coords) override;

private:
    // fastest speed at which the bumper may travel
    float max_movement_speed_;
    double cpu_smash_rate_;
    double cpu_dizzy_rate_;
    double cpu_monkey_rate_;
    double cpu_brittle_rate_;
    double cpu_random_rate_;

};

}








#endif //UNFAIR_PONG_CPU_BUMPER_H
