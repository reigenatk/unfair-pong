
#ifndef NAIVE_BAYES_SIMULATION_H
#define NAIVE_BAYES_SIMULATION_H


#include "../../../../include/glm/glm.hpp"
#include <string>
#include <vector>

using glm::vec2;

namespace unfairpong {

class Ball {
    public:


    private:
    vec2 position_;
    vec2 velocity_;
    std::vector<double> color_; // a vector of three doubles for r, g, b
    double radius_;


};

}

#endif //NAIVE_BAYES_SIMULATION_H
