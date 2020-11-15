
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
    Ball(vec2 starting_position, vec2 starting_velocity, cinder::Color color, double radius);

    vec2& GetPosition();
    vec2& GetVelocity();
    double GetRadius();
    void Draw() const;
    void UpdatePositionWithVelocity();


    private:
    vec2 position_;
    vec2 velocity_;
    cinder::Color color_; // a vector of three doubles for r, g, b
    double radius_;


};

}

#endif //NAIVE_BAYES_SIMULATION_H
