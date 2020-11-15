#include <visualizer/game.h>
#include <random>
#include <math.h>

using glm::vec2;
using glm::dot;
using glm::distance;
using glm::length;

namespace unfairpong {

namespace visualizer {

    Game::Game() {

    }

    UserBumper& Game::GetUserBumper() {
        return user_bumper_;
    }

    Game::Game(vec2 top_left, double length, double height) {
        top_left_corner_ = top_left;
        length_ = length;
        height_ = height;
        bottom_wall_ = top_left_corner_.y + height;
        right_wall_ = top_left_corner_.x + length;
        top_wall_ = top_left_corner_.y;
        left_wall_ = top_left_corner_.x;
        is_game_running_ = true;
        is_round_running_ = false;

        user_score_ = 0;
        cpu_score_ = 0;


        user_bumper_ = UserBumper(vec2((left_wall_ + right_wall_) / 2.0, bottom_wall_),
                                100, cinder::Color(1, 0, 0),
                                10, (float) left_wall_, (float) right_wall_);
        cpu_bumper_ = CpuBumper(vec2((left_wall_ + right_wall_) / 2.0, top_wall_), 100,
                                  cinder::Color(0, 1, 0), 10, 10, float(left_wall_), (float) right_wall_);

        vec2 random_velocity = vec2(GenerateRandomDouble(starting_velocity_cap_), GenerateRandomDouble(starting_velocity_cap_));
        ball_in_play = Ball(vec2((left_wall_ + right_wall_) / 2.0, (top_wall_+bottom_wall_) / 2.0),
                            random_velocity, cinder::Color(0, 0, 1), 5);

    }

    double Game::GenerateRandomDouble(double absolute_value_limit) {
        std::uniform_real_distribution<double> dist1(-absolute_value_limit, absolute_value_limit);

        //Mersenne Twister: Good quality random number generator
        std::mt19937 rng;
        //Initialize with non-deterministic seeds
        rng.seed(std::random_device{}());

        return dist1(rng);
    }

    vec2 Game::RandomVelocityGivenSpeed(double speed_desired, bool positive_y_velocity) {
        double new_y_vel;
        double new_x_vel;
        if (positive_y_velocity) {
            new_y_vel = GenerateRandomDouble(speed_desired);
            if (new_y_vel < 0) {
                new_y_vel = -new_y_vel;
            }
            double remaining_magnitude = sqrt(pow(speed_desired, 2) - pow(new_y_vel, 2));

            double is_x_vel_positive = GenerateRandomDouble(1);
            if (is_x_vel_positive > 0.5) {
                new_x_vel = remaining_magnitude;
            }
            else {
                new_x_vel = -remaining_magnitude;
            }
        }
        else {
            new_y_vel = GenerateRandomDouble(speed_desired);
            if (new_y_vel > 0) {
                new_y_vel = -new_y_vel;
            }
            double remaining_magnitude = sqrt(pow(speed_desired, 2) - pow(new_y_vel, 2));
            double is_x_vel_positive = GenerateRandomDouble(1);
            if (is_x_vel_positive > 0.5) {
                new_x_vel = remaining_magnitude;
            }
            else {
                new_x_vel = -remaining_magnitude;
            }

        }
        return vec2(new_x_vel, new_y_vel);
    }

    void Game::HandleMovement(const vec2 &mouse_coords) {
        user_bumper_.SetBumperCenter(mouse_coords);
    }

    void Game::Draw() {
        ci::gl::drawStringCentered("Control with the left and right arrow keys",
                                   vec2((left_wall_ + right_wall_) / 2.0, bottom_wall_ + 20),
                                   ci::Color("black"), ci::Font("Helvetica", 15));

        ci::gl::drawStringCentered("Ball velocity" + std::to_string(length(ball_in_play.GetVelocity())),
                                   vec2((left_wall_ + right_wall_) / 2.0, bottom_wall_ + 50),
                                   ci::Color("black"), ci::Font("Helvetica", 15));

        vec2 bottom_right_corner = top_left_corner_ + vec2(length_, height_);
        ci::gl::color(ci::Color("black"));
        ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
        ci::gl::drawStrokedRect(pixel_bounding_box);

        cpu_bumper_.Draw();
        user_bumper_.Draw();
        ball_in_play.Draw();

    }

    void Game::GameReset() {
        // reset bumpers into original positions
        user_bumper_ = UserBumper(vec2((left_wall_ + right_wall_) / 2.0, bottom_wall_),
                                  100, cinder::Color(1, 0, 0),
                                  10, (float) left_wall_, (float) right_wall_);
        cpu_bumper_ = CpuBumper(vec2((left_wall_ + right_wall_) / 2.0, top_wall_), 100,
                                cinder::Color(0, 1, 0), 10, 10, float(left_wall_), (float) right_wall_);

        vec2 random_velocity = vec2(GenerateRandomDouble(starting_velocity_cap_), GenerateRandomDouble(starting_velocity_cap_));
        ball_in_play = Ball(vec2((left_wall_ + right_wall_) / 2.0, (top_wall_+bottom_wall_) / 2.0),
                            random_velocity, cinder::Color(0, 0, 1), 5);
    }

    void Game::CheckIfPlayerScored() {
        vec2& current_ball_position = ball_in_play.GetPosition();
        if (abs(current_ball_position.y - bottom_wall_) < ball_in_play.GetRadius()) {
            // CPU scored
            cpu_score_++;

        }
        else if (abs(current_ball_position.y - top_wall_) < ball_in_play.GetRadius()) {
            // User scored
            user_score_++;
        }
    }

    void Game::ExecuteBallWallCollision() {
        // left wall
        vec2& current_ball_position = ball_in_play.GetPosition();
        vec2& current_ball_velocity = ball_in_play.GetVelocity();

        if (abs(current_ball_position.x - left_wall_) < ball_in_play.GetRadius() && current_ball_velocity.x < 0) {
            current_ball_velocity.x = - current_ball_velocity.x;
        }
        // right wall
        if (abs(current_ball_position.x - right_wall_) < ball_in_play.GetRadius() && current_ball_velocity.x > 0) {
            current_ball_velocity.x = - current_ball_velocity.x;
        }
    }

    void Game::UpdateBall() {
        ball_in_play.UpdatePositionWithVelocity();
    }

    void Game::UpdateCpuBumper() {
        cpu_bumper_.MakeMovementDecision(ball_in_play.GetPosition(), ball_in_play.GetVelocity());
    }

    void Game::ExecuteBallUserBumperCollision() {
        vec2& current_ball_position = ball_in_play.GetPosition();
        vec2& current_ball_velocity = ball_in_play.GetVelocity();
        double bumper_thickness = user_bumper_.GetBumperThickness();
        double bumper_length = user_bumper_.GetBumperLength();
        vec2& bumper_center = user_bumper_.GetBumperCenter();

        // if it intersects with the vertical height of our bumper
        // as well as in the range of our bumper length then it must be intersecting
        if (abs(current_ball_position.y - bottom_wall_) < bumper_thickness &&
                                abs(current_ball_position.x - bumper_center.x) < (float) (bumper_length / 2.0)) {
            current_ball_velocity = RandomVelocityGivenSpeed(length(current_ball_velocity) + 1, false);
        }
    }

    void Game::ExecuteBallCpuBumperCollision() {
        vec2& current_ball_position = ball_in_play.GetPosition();
        vec2& current_ball_velocity = ball_in_play.GetVelocity();
        double bumper_thickness = cpu_bumper_.GetBumperThickness();
        double bumper_length = cpu_bumper_.GetBumperLength();
        vec2& bumper_center = cpu_bumper_.GetBumperCenter();

        if (abs(current_ball_position.y - top_wall_) < bumper_thickness &&
            abs(current_ball_position.x - bumper_center.x) < (float) (bumper_length / 2.0)) {
            current_ball_velocity = RandomVelocityGivenSpeed(length(current_ball_velocity) + 1, true);
        }
    }

    void Game::UpdateUserBumper() {
        user_bumper_.ExecuteTimeStep();
    }

}  // namespace visualizer

}  // namespace naivebayes
