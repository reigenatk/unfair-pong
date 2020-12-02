
#ifndef NAIVE_BAYES_SIMULATION_H
#define NAIVE_BAYES_SIMULATION_H


#include "../../../../include/glm/glm.hpp"
#include <string>
#include <vector>
#include <cinder/gl/gl.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "user_bumper.h"
#include "cpu_bumper.h"

using glm::vec2;

namespace unfairpong {

class Ball {
    public:

    Ball();
    Ball(vec2 starting_position, vec2 starting_velocity, cinder::Color color,
         double radius, double user_smash_rate, double cpu_smash_rate, double cpu_dizzy_rate, double cpu_monkey_rate,
         double cpu_brittle_rate, double difficulty_increment);

    vec2& GetPosition();
    vec2& GetVelocity();
    double GetRadius() const;
    cinder::Color GetColor() const;

    void Draw();
    void UpdatePositionWithVelocity(vec2 farther_user_corner);
    void ChangeIntoRandomColor();

    /**
     * The following two methods execute any collisions, if they are colliding
     */
    void CollideWithUserBumper(UserBumper& user_bumper, float left_wall_x, float right_wall_x, float top_wall_y, float bottom_wall_y);
    void CollideWithCpuBumper(CpuBumper& cpu_bumper, float left_wall_x, float right_wall_x, float top_wall_y, float bottom_wall_y);

    // reset all the effect variables to false
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
    cinder::Color color_;
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
    double cpu_brittle_rate_;
    bool is_brittle_ball_;

    // how much faster the ball gets with each consecutive bumper collision
    double difficulty_increment_;

    // the fastest a ball can travel, need this because once the ball starts going too fast
    // collisions stop working
    float max_ball_velocity;
    cinder::gl::Texture2dRef effect_image_;
};

}

#endif //NAIVE_BAYES_SIMULATION_H
