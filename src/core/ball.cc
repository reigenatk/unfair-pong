
#include <core/ball.h>
#include <iostream>

using std::cout;

namespace idealgas {

Particle::Particle(const vec2& initial_pos, const vec2& initial_vel, double radius, double mass, std::vector<double> color) {
    position_ = initial_pos;
    velocity_ = initial_vel;
    radius_ = radius;
    mass_ = mass;
    color_ = color;
}

vec2 Particle::GetPosition() const {
    return position_;
}

vec2 Particle::GetVelocity() const {
    return velocity_;
}

double Particle::GetRadius() const {
    return radius_;
}

std::vector<double> Particle::GetColor() const {
    return color_;
}

double Particle::GetMass() const {
    return mass_;
}

void Particle::SetVelocity(const vec2& new_vel) {
    velocity_ = new_vel;
}

void Particle::Move() {
    position_ += velocity_;
}

bool Particle::IsParticleCollidingWith(const Particle& other) const {
    if (dot((velocity_ - other.GetVelocity()), (position_ - other.GetPosition())) < 0) {
        // check if the radii are within r1 + r2 of each other
        if (distance(position_, other.GetPosition()) <= radius_ + other.GetRadius()) {
            return true;
        }
    }
    return false;
}

void Particle::Collide(Particle &other) {

    vec2 new_a_velocity = velocity_;
    vec2 new_b_velocity = other.GetVelocity();

    double a_scalar = dot((velocity_ - other.GetVelocity()), (position_ - other.GetPosition())) /
                      pow((length(position_ - other.GetPosition())), 2);
    double b_scalar = dot((other.GetVelocity() - velocity_), (other.GetPosition() - position_)) /
                      pow((length(other.GetPosition() - position_)), 2);

    // add this for mass

    a_scalar *= (2*other.GetMass()) / (mass_ + other.GetMass());
    b_scalar *= (2*mass_) / (mass_ + other.GetMass());

    new_a_velocity -= static_cast<float>(a_scalar) * (position_ - other.GetPosition());
    new_b_velocity -= static_cast<float>(b_scalar) * (other.GetPosition() - position_);

    velocity_ = new_a_velocity;
    other.SetVelocity(new_b_velocity);
}

}