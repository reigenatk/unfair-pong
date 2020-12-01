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

    // images
    cinder::gl::Texture2dRef sad_emoji_ = cinder::gl::Texture2d::create
            (cinder::loadImage("../../../data/whygodwhy.png"));

}

void UnfairPongApp::update() {
    // if someone has won then stop updating
    if (unfair_pong_instance.HasUserWon() || unfair_pong_instance.HasCpuWon()) {
        return;
    }

    // if game has started and a round is running, update
    if (unfair_pong_instance.IsDifficultySelected() && unfair_pong_instance.IsRoundRunning()) {
        unfair_pong_instance.UpdateAll();
    }
}

void UnfairPongApp::draw() {

    ci::Color8u background_color(255, 246, 148);  // light yellow
    ci::gl::clear(background_color);
    unfair_pong_instance.Draw();

    ci::gl::drawStringCentered(std::to_string(length(unfair_pong_instance.GetBall().GetVelocity())),
                               vec2(50, 10),
                               ci::Color("black"), ci::Font("Helvetica", 30));

    ci::gl::drawStringCentered("Unfair Pong",
                               vec2(window_length_ / 2.0, 30),
                               ci::Color("black"), ci::Font("Helvetica", 30));

    ci::gl::color(ci::Color("white"));

    ci::gl::draw(sad_emoji_, vec2(window_length_ / 2.0 + 60, 10));

    // prompt user to pick difficulty if not selected yet
    if (!unfair_pong_instance.IsDifficultySelected()) {
        ci::gl::drawStringCentered("Press: 1 for easy, 2 for medium, 3 for hard, 4 for unfair",
                                   vec2(window_length_ / 2.0, 150),
                                   ci::Color("black"), ci::Font("Helvetica", 15));
    }

    // start round message (or win screen if someone has won)
    if (unfair_pong_instance.HasCpuWon()) {
        ci::gl::drawStringCentered("CPU wins!",
                                   vec2(window_length_ / 2.0, 150),
                                   ci::Color("black"), ci::Font("Helvetica", 15));
    }
    else if (unfair_pong_instance.HasUserWon()) {
        ci::gl::drawStringCentered("You win!",
                                   vec2(window_length_ / 2.0, 150),
                                   ci::Color("black"), ci::Font("Helvetica", 15));
    }
    else if (unfair_pong_instance.IsDifficultySelected() && !unfair_pong_instance.IsRoundRunning()) {
        ci::gl::drawStringCentered("Press SPACE to start round",
                                   vec2(window_length_ / 2.0, 150),
                                   ci::Color("black"), ci::Font("Helvetica", 15));
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
