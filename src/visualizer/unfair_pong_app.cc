#include <visualizer/unfair_pong_app.h>
#include <json.hpp>
#include <chrono>
#include <thread>
#include <string>
#include <set>

using std::cout;
// my json deserializer: https://github.com/nlohmann/json
using json = nlohmann::json;

namespace unfairpong {

namespace visualizer {

UnfairPongApp::UnfairPongApp() {

}

void UnfairPongApp::setup() {
    // set the window sizes but do not configure the game settings until user selects difficulty
    // using arrow keys

    ci::app::setWindowSize((int) window_length_, (int) window_height_);

    double box_length = 600;
    double box_height = 600;
    unfair_pong_instance = Game(vec2(100, 100), box_length, box_height);


}

void UnfairPongApp::update() {
    // no point to update if game hasnt started yet

    if (unfair_pong_instance.IsDifficultySelected() && unfair_pong_instance.IsRoundRunning()) {
        unfair_pong_instance.UpdateBall();
        unfair_pong_instance.UpdateUserBumper();
        unfair_pong_instance.UpdateCpuBumper();

        unfair_pong_instance.CheckIfPlayerScored();

        unfair_pong_instance.ExecuteBallWallCollision();
        unfair_pong_instance.ExecuteBallUserBumperCollision();
        unfair_pong_instance.ExecuteBallCpuBumperCollision();
    }
}

void UnfairPongApp::draw() {

    ci::Color8u background_color(255, 246, 148);  // light yellow
    ci::gl::clear(background_color);
    unfair_pong_instance.Draw();

    ci::gl::drawStringCentered("Unfair Pong",
                               vec2(window_length_ / 2.0, 30),
                               ci::Color("black"), ci::Font("Helvetica", 30));
    ci::gl::drawStringCentered("Press: 1 for easy, 2 for medium, 3 for hard, 4 for unfair",
                               vec2(window_length_ / 2.0, 65),
                               ci::Color("black"), ci::Font("Helvetica", 15));

    if (!unfair_pong_instance.IsDifficultySelected()) {
        ci::gl::drawStringCentered("Please select your difficulty using 1, 2, 3, 4",
                                   vec2(window_length_ / 2.0, 150),
                                   ci::Color("black"), ci::Font("Helvetica", 30));
    }

    if (unfair_pong_instance.IsDifficultySelected() && !unfair_pong_instance.IsRoundRunning()) {
        ci::gl::drawStringCentered("Press SPACE to start round",
                                   vec2(window_length_ / 2.0, 150),
                                   ci::Color("black"), ci::Font("Helvetica", 30));
    }
}

void UnfairPongApp::mouseDown(ci::app::MouseEvent event) {
    unfair_pong_instance.HandleMouseMovement(event.getPos());
}

void UnfairPongApp::mouseDrag(ci::app::MouseEvent event) {
    unfair_pong_instance.HandleMouseMovement(event.getPos());
}

void UnfairPongApp::keyDown(ci::app::KeyEvent event) {
    switch (event.getCode()) {
        case ci::app::KeyEvent::KEY_1: {
            // only allow us to select a difficulty if one hasn't already been selected.
            if (unfair_pong_instance.IsDifficultySelected() == false) {
                unfair_pong_instance.SelectDifficulty("easy");
            }
            break;
        }
        case ci::app::KeyEvent::KEY_2: {
            if (unfair_pong_instance.IsDifficultySelected() == false) {
                unfair_pong_instance.SelectDifficulty("medium");
            }
            break;
        }
        case ci::app::KeyEvent::KEY_3: {
            if (unfair_pong_instance.IsDifficultySelected() == false) {
                unfair_pong_instance.SelectDifficulty("hard");
            }
            break;
        }
        case ci::app::KeyEvent::KEY_4: {
            if (unfair_pong_instance.IsDifficultySelected() == false) {
                unfair_pong_instance.SelectDifficulty("unfair");
            }
            break;
        }
        case ci::app::KeyEvent::KEY_RIGHT: {
            unfair_pong_instance.GetUserBumper().MoveBumperRight();
            break;
        }
        case ci::app::KeyEvent::KEY_LEFT: {
            unfair_pong_instance.GetUserBumper().MoveBumperLeft();
            break;
        }
        case ci::app::KeyEvent::KEY_SPACE: {
            unfair_pong_instance.StartNewRound();
            break;
        }
    }
}

}  // namespace visualizer

}  // namespace naivebayes
