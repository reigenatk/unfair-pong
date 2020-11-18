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
    unfair_pong_instance = Game(vec2(100, 100), 600, 600);


}

void UnfairPongApp::update() {
    // no point to update if game hasnt started yet

    if (unfair_pong_instance.IsGameRunning()) {
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
    ci::gl::drawStringCentered("Press: 1 for easy, 2 for medium, 3 for hard",
                               vec2(window_length_ / 2.0, 65),
                               ci::Color("black"), ci::Font("Helvetica", 15));
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
            unfair_pong_instance.SelectDifficultyAndStart("easy");
            break;
        }
        case ci::app::KeyEvent::KEY_2: {
            unfair_pong_instance.SelectDifficultyAndStart("medium");
            break;
        }
        case ci::app::KeyEvent::KEY_3: {
            unfair_pong_instance.SelectDifficultyAndStart("hard");
            break;
        }
        case ci::app::KeyEvent::KEY_4: {
            unfair_pong_instance.SelectDifficultyAndStart("unfair");
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
    }
}

}  // namespace visualizer

}  // namespace naivebayes
