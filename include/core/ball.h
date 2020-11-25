
#ifndef NAIVE_BAYES_SIMULATION_H
#define NAIVE_BAYES_SIMULATION_H


#include "../../../../include/glm/glm.hpp"
#include <string>
#include <vector>
#include <cinder/gl/gl.h>

using glm::vec2;

namespace unfairpong {

class Ball {
    public:

    Ball();
    Ball(vec2 starting_position, vec2 starting_velocity, cinder::Color color,
         double radius, double user_smash_rate, double cpu_smash_rate, double cpu_dizzy_rate, double cpu_monkey_rate, double difficulty_increment);

    vec2& GetPosition();
    vec2& GetVelocity();
    double GetRadius();
    cinder::Color GetColor();

    void Draw() const;
    void UpdatePositionWithVelocity(vec2 farther_user_corner);
    void ChangeIntoRandomColor();

    // these methods decide the new velocities of the ball upon cpu and user collision
    void CollideWithUserBumper(vec2 center_of_user_bumper, float left_wall_x, float right_wall_x, float top_wall_y);
    void CollideWithCpuBumper(vec2 center_of_user_bumper, float left_wall_x, float right_wall_x, float bottom_wall_y);

    void ResetAllEffects();

    /**
     * Given a target position for the ball to hit as a vec2 and a desired speed to travel towards it at
     * Generate a vector that meets both requirements
     */
    vec2 VelocityGivenTargetAndSpeed(vec2 target_pos, double desired_velocity_of_ball);

    /**
     * Resets the ball to a certain position, need to call this after each round ends
     */
    void ResetForNewRound(vec2 new_position, vec2 new_velocity);

    private:
    vec2 position_;
    vec2 velocity_;
    cinder::Color color_; // a vector of three doubles for r, g, b
    double radius_;

    // a smash ball goes adds more velocity than usual onto the ball
    // it also does NOT collide with a wall, instead travels straight towards the goal
    double smash_velocity_increase_;
    double user_smash_rate_;
    double cpu_smash_rate_;
    bool is_smash_ball_;
    double cpu_dizzy_rate_;
    bool is_dizzy_ball_;
    double cpu_monkey_rate_;
    bool is_monkey_ball_;

    // how much faster the ball gets with each consecutive bumper collision
    double difficulty_increment_;

    // the fastest a ball can travel, need this because once the ball starts going too fast
    // collisions stop working
    float max_ball_velocity;

};

}

#endif //NAIVE_BAYES_SIMULATION_H
