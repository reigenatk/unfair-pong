
#ifndef NAIVE_BAYES_SIMULATION_H
#define NAIVE_BAYES_SIMULATION_H


#include "../../../../include/glm/glm.hpp"
#include <string>
#include <vector>

using glm::vec2;

namespace idealgas {

class Particle {
    public:

    Particle(const vec2& initial_pos, const vec2& initial_vel, double radius_, double mass_, std::vector<double> color);
    vec2 GetPosition() const;
    vec2 GetVelocity() const;
    double GetRadius() const;
    std::vector<double> GetColor() const;
    double GetMass() const;

    // I included this because its needed for the Collide method
    void SetVelocity(const vec2& new_vel);

    /**
   * Check to see if other particle is colliding with this one given a reference to other particle
   */
    bool IsParticleCollidingWith(const Particle& other) const;

    /**
     * Collide this particle with another particle using the formulas
     * @param other
     */
    void Collide(Particle& other);

    /**
     * This method changes the position by doing position += velocity
     */
    void Move();

    private:
    vec2 position_;
    vec2 velocity_;
    std::vector<double> color_; // a vector of three doubles for r, g, b
    double radius_;
    double mass_;

};

}

#endif //NAIVE_BAYES_SIMULATION_H
