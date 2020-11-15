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

        user_score_ = 0;
        cpu_score_ = 0;
        points_to_win_ = 10;

        user_bumper_ = UserBumper(vec2((left_wall_ + right_wall_) / 2.0, bottom_wall_),
                                100, cinder::Color(1, 0, 0),
                                10, (float) left_wall_, (float) right_wall_);
        cpu_bumper_ = CpuBumper(vec2((left_wall_ + right_wall_) / 2.0, top_wall_), 100,
                                  cinder::Color(0, 1, 0), 10);

    }

    void Game::Draw() const {
        ci::gl::drawStringCentered("Control with the left and right arrow keys",
                                   vec2((left_wall_ + right_wall_) / 2.0, bottom_wall_ + 20),
                                   ci::Color("black"), ci::Font("Helvetica", 15));

        vec2 bottom_right_corner = top_left_corner_ + vec2(length_, height_);
        ci::gl::color(ci::Color("black"));
        ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner);
        ci::gl::drawStrokedRect(pixel_bounding_box);

        cpu_bumper_.Draw();
        user_bumper_.Draw();

    }



    void Game::UpdateBall() {

    }

    void Game::UpdateCpuBumper() {

    }

    void Game::UpdateUserBumper() {
        user_bumper_.ExecuteTimeStep();
    }

}  // namespace visualizer

}  // namespace naivebayes
