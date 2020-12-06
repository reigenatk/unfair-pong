
#ifndef NAIVE_BAYES_SIMULATION_H
#define NAIVE_BAYES_SIMULATION_H


#include "../../../../include/glm/glm.hpp"
#include <string>
#include <vector>
#include <cinder/gl/gl.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "bumper.h"

using glm::vec2;

namespace unfairpong {

class Ball {
    public:

    Ball();
    Ball(vec2 starting_position, vec2 starting_velocity, cinder::Color color, double radius, double difficulty_increment_);

    vec2& GetPosition();
    vec2& GetVelocity();
    double GetRadius() const;
    cinder::Color GetColor() const;

    void Draw();

    /**
     * Update the new position of the ball, called every tick
     * @param farther_user_corner For the monkey ball, we must know which direction the ball ought to travel at any moment so we pass this in
     */
    void UpdatePositionWithVelocity(vec2 farther_user_corner);

    /**
     * The following two methods execute any collisions, if they are colliding
     */
    void CollideWithTopBumper(Bumper* top_bumper, float left_wall_x, float right_wall_x, float top_wall_y, float bottom_wall_y);
    void CollideWithBottomBumper(Bumper* bottom_bumper, float left_wall_x, float right_wall_x, float top_wall_y, float bottom_wall_y);

    /**
     * Resets all the effect variables to false. This is only ever be called after a collision occurs
     */
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
    BallType type_of_ball_;

    // here are some general pieces of info about collisions that don't really belong to a specific bumper
    double smash_velocity_increase_;
    double frames_elapsed_;
    double frames_until_random_;

    // how much faster the ball gets with each consecutive bumper collision (this is regardless of what kind of ball it is)
    double difficulty_increment_;

    // the fastest a ball can travel, need this because once the ball starts going too fast
    // collisions stop working and balls start going through bumpers
    float max_ball_velocity;

    // the image to accompany the ball if the ball isn't normal
    cinder::gl::Texture2dRef effect_image_;
};

}

#endif //NAIVE_BAYES_SIMULATION_H
