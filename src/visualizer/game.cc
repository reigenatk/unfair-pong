#include <visualizer/game.h>
#include <random>
#include <math.h>
#include <json.hpp>
#include <string>

using glm::vec2;
using glm::dot;
using glm::distance;
using glm::length;
using std::string;

using json = nlohmann::json;

namespace unfairpong {

namespace visualizer {

Game::Game() {

}

UserBumper& Game::GetUserBumper() {
    return user_bumper_;
}

// this method is a little long mainly to avoid using unnecessary member variables
void Game::SelectDifficulty(string difficulty) {
    string path = "../../../data/";
    path += difficulty;
    path += ".json";
    std::ifstream i(path);
    json j;
    i >> j;

    float radius_of_ball_;
    cinder::Color color_of_ball_;
    cinder::Color color_of_user_bumper_;
    cinder::Color color_of_cpu_bumper_;
    float max_cpu_velocity_;
    double difficulty_increment_;

    double user_bumper_length_;
    double cpu_bumper_length_;

    // powerups
    double user_smash_rate;
    double cpu_smash_rate;

    radius_of_ball_ = j["radius_of_ball_"];
    points_to_win_ = j["points_to_win_"];

    // deserialize from vec of doubles to cinder::Color
    vector<float> ball_color, user_bumper_color, cpu_bumper_color;
    ball_color = j["color_of_ball_"].get<vector<float>>();
    color_of_ball_ = cinder::Color(ball_color[0], ball_color[1], ball_color[2]);
    user_bumper_color = j["color_of_user_bumper_"].get<vector<float>>();
    color_of_user_bumper_ = cinder::Color(user_bumper_color[0], user_bumper_color[1], user_bumper_color[2]);
    cpu_bumper_color = j["color_of_cpu_bumper_"].get<vector<float>>();
    color_of_cpu_bumper_ = cinder::Color(cpu_bumper_color[0], cpu_bumper_color[1], cpu_bumper_color[2]);

    starting_ball_velocity_cap_ = j["starting_ball_velocity_cap_"];
    starting_ball_velocity_floor_ = j["starting_ball_velocity_floor_"];
    difficulty_increment_ = j["difficulty_increment_"];
    max_cpu_velocity_ = j["max_cpu_velocity_"];

    user_bumper_length_ = j["user_bumper_length_"];
    cpu_bumper_length_ = j["cpu_bumper_length_"];

    user_smash_rate = j["user_smash_rate"];
    cpu_smash_rate = j["cpu_smash_rate"];

    // now create all the objects and set game to running, now that we have selected a difficulty!

    is_difficulty_selected_ = true;

    // I need this value to be relatively large or else at high ball velocities, the game will mistakenly think
    // that a user scored because the paddle isn't large enough to collide with ball in time
    double thickness_of_bumper = 20;

    user_bumper_ = UserBumper(vec2((GetLeftWallX() + GetRightWallX()) / 2.0, GetBottomWallY()),
                              user_bumper_length_, color_of_user_bumper_,
                              thickness_of_bumper, (float) GetLeftWallX(), (float) GetRightWallX());
    cpu_bumper_ = CpuBumper(vec2((GetLeftWallX() + GetRightWallX()) / 2.0, GetTopWallY()), cpu_bumper_length_,
                            color_of_cpu_bumper_, thickness_of_bumper, max_cpu_velocity_, float(GetLeftWallX()), (float) GetRightWallX());

    // we have a certain range that will be the starting speed of the ball each round
    double starting_speed_of_ball = GenerateRandomDoubleBetween(starting_ball_velocity_floor_, starting_ball_velocity_cap_);

    // now generate some random vec2 velocity with this speed and send it towards the CPU to begin with
    // so user has time to react
    vec2 random_velocity = RandomVelocityGivenSpeed(starting_speed_of_ball, false);

    ball_in_play = Ball(vec2((GetLeftWallX() + GetRightWallX()) / 2.0, (GetTopWallY() + GetBottomWallY()) / 2.0),
                        random_velocity, color_of_ball_, radius_of_ball_, user_smash_rate, cpu_smash_rate, difficulty_increment_);
}

Game::Game(vec2 top_left, double length, double height) {
    top_left_corner_ = top_left;
    length_ = length;
    height_ = height;
    is_round_running_ = false;
    is_difficulty_selected_ = false;

    user_score_ = 0;
    cpu_score_ = 0;

}

void Game::UpdateAll() {
    UpdateBall();
    UpdateUserBumper();
    UpdateCpuBumper();

    CheckIfPlayerScored();

    ExecuteBallWallCollision();
    ExecuteBallUserBumperCollision();
    ExecuteBallCpuBumperCollision();
}

bool Game::IsDifficultySelected() {
    return is_difficulty_selected_;
}

bool Game::IsRoundRunning() {
    return is_round_running_;
}

bool Game::HasUserWon() {
    return (user_score_ == points_to_win_);
}

bool Game::HasCpuWon() {
    return (cpu_score_ == points_to_win_);
}

double Game::GetLeftWallX() {
    return top_left_corner_.x;
}

double Game::GetRightWallX() {
    return top_left_corner_.x + length_;
}

double Game::GetBottomWallY() {
    return top_left_corner_.y + height_;
}

double Game::GetTopWallY() {
    return top_left_corner_.y;
}

Ball Game::GetBall() {
    return ball_in_play;
}

double Game::GenerateRandomDouble(double absolute_value_limit) {
    std::uniform_real_distribution<double> dist1(-absolute_value_limit, absolute_value_limit);

    //Mersenne Twister: Good quality random number generator
    std::mt19937 rng;
    //Initialize with non-deterministic seeds
    rng.seed(std::random_device{}());

    return dist1(rng);
}

double Game::GenerateRandomDoubleBetween(double lb, double ub) {
    std::uniform_real_distribution<double> dist1(lb, ub);

    //Mersenne Twister: Good quality random number generator
    std::mt19937 rng;
    //Initialize with non-deterministic seeds
    rng.seed(std::random_device{}());

    return dist1(rng);
}

vec2 Game::RandomVelocityGivenSpeed(double speed_desired, bool positive_y_velocity) {
    double new_y_vel;
    double new_x_vel;

    // this value dictates that the y-velocities of randomly generated velocities must all be LARGER
    // than this value. The reason I added this is because I can genereate a velocity like (5, 0.1) which will take
    // forever to bounce to the opposing bumper, since walls do not affect y-velocity.
    // and that is bad user experience since they need to wait really long
    double y_velocity_lower_bound = 3;

    if (positive_y_velocity) {
        new_y_vel = GenerateRandomDouble(speed_desired);
        while (abs(new_y_vel) < y_velocity_lower_bound) {
            new_y_vel = GenerateRandomDouble(speed_desired);
        }
        if (new_y_vel < 0) {
            new_y_vel = -new_y_vel;
        }
        double remaining_magnitude = sqrt(pow(speed_desired, 2) - pow(new_y_vel, 2));

        // flip a coin to see if we get positive or negative x-vel
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
        while (abs(new_y_vel) < y_velocity_lower_bound) {
            new_y_vel = GenerateRandomDouble(speed_desired);
        }
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

void Game::HandleMouseMovement(const vec2 &mouse_coords) {
    user_bumper_.SteerBumperWithMouse(mouse_coords);
}

void Game::DrawInstructions() {
    ci::gl::drawStringCentered("Control with arrow keys OR dragging mouse",
                               vec2((GetLeftWallX() + GetRightWallX()) / 2.0, GetBottomWallY() + 20),
                               ci::Color("black"), ci::Font("Helvetica", 15));
}

void Game::DrawScore() {
    float radius_of_ball_ = (float) ball_in_play.GetRadius();
    cinder::Color color_of_ball_ = ball_in_play.GetColor();

    vec2 ball_center_1 = vec2(GetRightWallX(), GetBottomWallY());
    ball_center_1 += vec2(radius_of_ball_, -radius_of_ball_);
    float gap = 1;
    ball_center_1 += vec2(gap, 0);

    for (size_t i = 0; i < user_score_; i++) {
        ci::gl::color(color_of_ball_);
        ci::gl::drawSolidCircle(ball_center_1, radius_of_ball_);
        ball_center_1 += vec2(0, -radius_of_ball_*2 - gap);
    }

    vec2 ball_center_2 = vec2(GetRightWallX(), GetTopWallY());
    ball_center_2 += vec2(radius_of_ball_, -radius_of_ball_);
    ball_center_2 += vec2(gap, 0);

    for (size_t i = 0; i < cpu_score_; i++) {
        ci::gl::color(color_of_ball_);
        ci::gl::drawSolidCircle(ball_center_2, radius_of_ball_);
        ball_center_2 += vec2(0, radius_of_ball_*2 + gap);
    }
}

void Game::Draw() {
    DrawInstructions();

    // boundaries
    vec2 bottom_right_corner = top_left_corner_ + vec2(length_, height_);
    ci::gl::color(ci::Color("black"));
    ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
    ci::gl::drawStrokedRect(pixel_bounding_box);

    DrawScore();

    cpu_bumper_.Draw();
    user_bumper_.Draw();
    ball_in_play.Draw();

}

bool Game::RollChance(double chance) {
    double d = GenerateRandomDoubleBetween(0, 1);
    return d < chance;
}

void Game::SetupNewRound() {

    double starting_speed_of_ball = GenerateRandomDoubleBetween(starting_ball_velocity_floor_, starting_ball_velocity_cap_);
    vec2 random_velocity = RandomVelocityGivenSpeed(starting_speed_of_ball, false);

    ball_in_play.ResetForNewRound(vec2((GetLeftWallX() + GetRightWallX()) / 2.0, (GetTopWallY() + GetBottomWallY()) / 2.0), random_velocity);
    cpu_bumper_.ResetForNewRound(vec2((GetLeftWallX() + GetRightWallX()) / 2.0, GetTopWallY()));
    user_bumper_.ResetForNewRound(vec2((GetLeftWallX() + GetRightWallX()) / 2.0, GetBottomWallY()));
}

void Game::StartNewRound() {
    is_round_running_ = true;
}

void Game::CheckIfPlayerScored() {
    vec2& current_ball_position = ball_in_play.GetPosition();
    if (current_ball_position.y > GetBottomWallY()) {
        // CPU scored
        cpu_score_++;
        is_round_running_ = false;

        if (HasCpuWon()) {
            // do not set up new round
        }
        else {
            SetupNewRound();
        }
    }
    else if (current_ball_position.y < GetTopWallY()) {
        // User scored
        user_score_++;
        is_round_running_ = false;

        if (HasUserWon()) {
            // do not set up new round
        }
        else {
            SetupNewRound();
        }
    }
}

void Game::ExecuteBallWallCollision() {
    // left wall
    vec2& current_ball_position = ball_in_play.GetPosition();
    vec2& current_ball_velocity = ball_in_play.GetVelocity();

    if (abs(current_ball_position.x - GetLeftWallX()) < ball_in_play.GetRadius() && current_ball_velocity.x < 0) {
        current_ball_velocity.x = - current_ball_velocity.x;
    }
    // right wall
    if (abs(current_ball_position.x - GetRightWallX()) < ball_in_play.GetRadius() && current_ball_velocity.x > 0) {
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
    double ball_radius = ball_in_play.GetRadius();
    double bumper_thickness = user_bumper_.GetBumperThickness();
    double bumper_length = user_bumper_.GetBumperLength();
    vec2& bumper_center = user_bumper_.GetBumperCenter();

    // if intersecting
    if (abs(current_ball_position.y - GetBottomWallY()) < bumper_thickness + (float) (ball_radius / 2.0) &&
                            abs(current_ball_position.x - bumper_center.x) < (float) (bumper_length / 2.0)) {
        ball_in_play.CollideWithUserBumper(bumper_center,
                                           (float) GetLeftWallX(),
                                           (float) GetRightWallX(),
                                           (float) GetTopWallY());
    }
}

void Game::ExecuteBallCpuBumperCollision() {
    vec2& current_ball_position = ball_in_play.GetPosition();
    vec2& current_ball_velocity = ball_in_play.GetVelocity();
    double ball_radius = ball_in_play.GetRadius();
    double bumper_thickness = cpu_bumper_.GetBumperThickness();
    double bumper_length = cpu_bumper_.GetBumperLength();
    vec2& bumper_center = cpu_bumper_.GetBumperCenter();

    if (abs(current_ball_position.y - GetTopWallY()) < bumper_thickness + (float) (ball_radius / 2.0) &&
        abs(current_ball_position.x - bumper_center.x) < (float) (bumper_length / 2.0)) {
        ball_in_play.CollideWithCpuBumper(bumper_center, (float) GetLeftWallX(),
                                          (float) GetRightWallX(),
                                          (float) GetBottomWallY());
    }
}

void Game::UpdateUserBumper() {
    user_bumper_.ExecuteTimeStep();
}

}  // namespace visualizer

}  // namespace naivebayes
