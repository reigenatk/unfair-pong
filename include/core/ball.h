
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
         double radius, double user_smash_rate, double cpu_smash_rate, double difficulty_increment);

    vec2& GetPosition();
    vec2& GetVelocity();
    double GetRadius();
    cinder::Color GetColor();

    void Draw() const;
    void UpdatePositionWithVelocity();
    void ChangeIntoRandomColor();

    void CollideWithUserBumper();
    void CollideWithCpuBumper();

    /**
     * Resets the ball to a certain position, need to call this after each round ends
     */
    void ResetForNewRound(vec2 new_position, vec2 new_velocity);

    private:
    vec2 position_;
    vec2 velocity_;
    cinder::Color color_; // a vector of three doubles for r, g, b
    double radius_;
    double user_smash_rate_;
    double cpu_smash_rate_;
    double difficulty_increment_;

};

}

#endif //NAIVE_BAYES_SIMULATION_H
