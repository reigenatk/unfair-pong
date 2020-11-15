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
    // configure everything in json files and set the file here!

    ci::app::setWindowSize((int) window_length_, (int) window_height_);
    unfair_pong_instance = Game(vec2(100, 100), 600, 600);


}

void UnfairPongApp::update() {
    unfair_pong_instance.UpdateBall();
    unfair_pong_instance.UpdateUserBumper();
    unfair_pong_instance.UpdateCpuBumper();

    unfair_pong_instance.CheckIfPlayerScored();
    unfair_pong_instance.ExecuteBallWallCollision();
    unfair_pong_instance.ExecuteBallUserBumperCollision();
    unfair_pong_instance.ExecuteBallCpuBumperCollision();
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

void UnfairPongApp::keyDown(ci::app::KeyEvent event) {
    switch (event.getCode()) {
        case ci::app::KeyEvent::KEY_1: {

        }
        case ci::app::KeyEvent::KEY_2: {

        }
        case ci::app::KeyEvent::KEY_3: {

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
