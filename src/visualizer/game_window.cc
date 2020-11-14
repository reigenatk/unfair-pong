#include <visualizer/box.h>
#include <random>
#include <math.h>

namespace idealgas {

namespace visualizer {

using glm::vec2;
using glm::dot;
using glm::distance;
using glm::length;

Box::Box() {

}

Box::Box(const glm::vec2 &top_left_corner, double length, double height):
    top_left_corner_(top_left_corner),
    length_of_box_(length),
    height_of_box_(height),
    y_bound(0) {
    if (length < 0 || height < 0 || top_left_corner.x < 0 || top_left_corner.y < 0) {
        throw std::invalid_argument("Box parameters must be non-negative");
    }
}

const vector<Particle>& Box::GetParticles() const {
    return particles_;
}

double Box::GetYBound() {
    return y_bound;
}

double Box::GenerateRandomDouble(double absolute_value_limit) {
        std::uniform_real_distribution<double> dist1(-absolute_value_limit, absolute_value_limit);

        //Mersenne Twister: Good quality random number generator
        std::mt19937 rng;
        //Initialize with non-deterministic seeds
        rng.seed(std::random_device{}());

        return dist1(rng);
}

void Box::AddParticleToBox(const Particle& p) {
    particles_.push_back(p);
}

void Box::SpawnParticles(size_t number_of_particles, double radius, double mass, vector<double> color,
                         double starting_velocity_cap, double spawn_gap) {

    if (radius <= 0) {
        throw std::invalid_argument("Radius must be positive");
    }
    if (starting_velocity_cap < 0) {
        throw std::invalid_argument("Velocity cap must be non-negative");
    }
    if (spawn_gap < 0) {
        throw std::invalid_argument("Spawn gap must be non-negative");
    }

    // calculate how many particles we can spawn

    double open_vertical_space = height_of_box_ - y_bound;

    double diameter = 2*radius;
    int particles_per_row = (int) floor(length_of_box_ / (diameter + spawn_gap));
    int particles_per_col = (int) floor(open_vertical_space / (diameter + spawn_gap));
    if (particles_per_col * particles_per_row < (int) number_of_particles) {
        throw std::invalid_argument("Not enough room for all particles");
    }

    vec2 spawn_top_left_corner = top_left_corner_ + vec2(0, y_bound);

    size_t particles_added = 0;
    for (int i = 1; i <= particles_per_col; i++) {
        for (int j = 1; j <= particles_per_row; j++) {
            // generate a random velocity whose magnitude is less than starting_velocity_cap

            double random_x_velocity = GenerateRandomDouble(starting_velocity_cap);

            double y_limit = sqrt(
                    (starting_velocity_cap * starting_velocity_cap) - (random_x_velocity * random_x_velocity));

            double random_y_velocity = GenerateRandomDouble(y_limit);

            double new_x = spawn_top_left_corner.x + (j * spawn_gap) + ((2*j - 1) * radius);
            double new_y = spawn_top_left_corner.y + (i * spawn_gap) + ((2*i - 1) * radius);
            vec2 center_of_new_particle(new_x, new_y);

            Particle new_particle(center_of_new_particle, vec2(random_x_velocity, random_y_velocity), radius, mass, color);
            particles_.push_back(new_particle);

            particles_added++;
            if (particles_added == number_of_particles) {
                y_bound += (diameter + spawn_gap);
                return;
            }
        }
        y_bound += (diameter + spawn_gap);
    }
}

void Box::ExecuteTimeStep() {
    for (auto& particle : particles_) {
        particle.Move();
    }
}

void Box::ExecuteParticleCollisions() {
    for (size_t i = 0; i < particles_.size(); i++) {
        for (size_t j = i+1; j < particles_.size(); j++) {
            if (particles_[i].IsParticleCollidingWith(particles_[j])) {
                particles_[i].Collide(particles_[j]);
            }
        }
    }
}

void Box::ExecuteParticleWallCollisions() {
    double left_wall = top_left_corner_.x;
    double right_wall = top_left_corner_.x + length_of_box_;
    double upper_wall = top_left_corner_.y;
    double bottom_wall = top_left_corner_.y + height_of_box_;

    for (size_t i = 0; i < particles_.size(); i++) {
        vec2 current_pos = particles_[i].GetPosition();
        vec2 current_vel = particles_[i].GetVelocity();
        double particle_radius = particles_[i].GetRadius();

        vec2 new_vel = current_vel;
        if (abs(left_wall - current_pos.x) <= particle_radius && current_vel.x < 0) {
            // left wall collision
            new_vel = vec2(-new_vel.x, new_vel.y);
        }
        else if (abs(right_wall - current_pos.x) <= particle_radius && current_vel.x > 0) {
            // right wall collision
            new_vel = vec2(-new_vel.x, new_vel.y);
        }

        if (abs(upper_wall - current_pos.y) <= particle_radius && current_vel.y < 0) {
            // upper wall collision
            new_vel = vec2(new_vel.x, -new_vel.y);
        }
        else if (abs(bottom_wall - current_pos.y) <= particle_radius && current_vel.y > 0) {
            // bottom wall collision
            new_vel = vec2(new_vel.x, -new_vel.y);
        }

        particles_[i].SetVelocity(new_vel);
    }
}

void Box::Draw() const {
    // should draw a box with correct length and height
    vec2 bottom_right_corner = top_left_corner_ + vec2(length_of_box_, height_of_box_);
    ci::gl::color(ci::Color("black"));
    ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
    ci::gl::drawStrokedRect(pixel_bounding_box);

    for (auto x : particles_) {
        vector<double> c = x.GetColor();
        ci::gl::color((float) c[0], (float) c[1], (float) c[2]);
        ci::gl::drawSolidCircle(x.GetPosition(), (float) x.GetRadius());
    }
}

}  // namespace visualizer

}  // namespace naivebayes
