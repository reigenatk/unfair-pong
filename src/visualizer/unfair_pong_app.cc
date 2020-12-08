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
    sad_emoji_ = cinder::gl::Texture2d::create
            (cinder::loadImage("../../../data/whygodwhy.png"));

    // default to this value, and when user selects a gamemode we can change
    game_mode_ = NotSelected;

}

void UnfairPongApp::update() {

    // if someone has won then stop updating
    if (unfair_pong_instance.HasTopPlayerWon() || unfair_pong_instance.HasBottomPlayerWon()) {
        return;
    }

    // else if game has started and a round is running, update
    if (unfair_pong_instance.IsDifficultySelected() && unfair_pong_instance.IsRoundRunning()) {
        unfair_pong_instance.UpdateAll();
    }

    // and if user is on loading screen, or hasnt pressed space to start round, then we just do nothing
}

void UnfairPongApp::draw() {
    // first prompt user to select a mode, CPU vs CPU or User vs CPU
    if (game_mode_ == NotSelected) {
        ci::Color8u background_color(0, 0, 0); // black
        ci::gl::clear(background_color);
        ci::gl::drawStringCentered("Choose a Gamemode",
                                   vec2(window_length_ / 2.0, window_height_ / 2.0 - 100),
                                   ci::Color(0,191,255), ci::Font("Raleway", 100));
        ci::gl::drawStringCentered("Press: ",
                                   vec2(window_length_ / 2.0, window_height_ / 2.0),
                                   ci::Color(0.9f, 0.124f, 0.27f), ci::Font("Raleway", 50));
        ci::gl::drawStringCentered("Z for singleplayer",
                                   vec2(window_length_ / 2.0, window_height_ / 2.0 + 75),
                                   ci::Color(0.9f, 0.8f, 0.2f), ci::Font("Raleway", 50));
        ci::gl::drawStringCentered("X for CPU vs CPU",
                                   vec2(window_length_ / 2.0, window_height_ / 2.0 + 150),
                                   ci::Color(0.2f, 0.9f, 0.4f), ci::Font("Raleway", 50));
        vec2 top_left_corner = vec2(window_length_ / 2.0, window_height_ / 2.0) + vec2(-200, 50);
        vec2 bottom_right_corner = vec2(window_length_ / 2.0, window_height_ / 2.0) + vec2(200, 200);
        ci::Rectf pixel_bounding_box(top_left_corner, bottom_right_corner);
        ci::gl::color(0.3f, 0.2f, 0.6f);
        ci::gl::drawStrokedRect(pixel_bounding_box);

    }
    else if (!unfair_pong_instance.IsDifficultySelected()) {
        // if user has selected gamemode, now ask for difficulty
        ci::Color8u background_color(0, 0, 0); // black
        ci::gl::clear(background_color);

        // draw a rectangle for the logo to sit on
        vec2 top_left_corner = vec2(window_length_ / 2.0, window_height_ / 2.0 - 100) + vec2(-250, -20);
        vec2 bottom_right_corner = vec2(window_length_ / 2.0, window_height_ / 2.0 - 100) + vec2(250, 80);
        ci::Rectf pixel_bounding_box(top_left_corner, bottom_right_corner);
        ci::gl::color(0.3f, 0.2f, 0.6f);
        ci::gl::drawSolidRect(pixel_bounding_box);

        // main logo thing
        ci::gl::drawStringCentered("Unfair Pong",
                                   vec2(window_length_ / 2.0, window_height_ / 2.0 - 100),
                                   ci::Color(0,191,255), ci::Font("Raleway", 100));

        ci::gl::drawStringCentered("(A CS 126 project by Richard Ma)",
                                   vec2(window_length_ / 2.0, window_height_ / 2.0),
                                   ci::Color(128, 0, 128), ci::Font("Yellowtail", 20));

        ci::gl::drawStringCentered("Press: 1 for easy, 2 for medium, 3 for hard, 4 for unfair",
                                   vec2(window_length_ / 2.0, window_height_ / 2.0 + 100),
                                   ci::Color(124, 252, 0), ci::Font("Helvetica", 40));
        std::string gamemode;
        if (game_mode_ == CPUVsCPU) {
            gamemode = "CPU vs CPU";
        }
        else {
            gamemode = "User vs CPU";
        }
        ci::gl::drawStringCentered("Gamemode: " + gamemode,
                                   vec2(window_length_ / 2.0, window_height_ / 2.0 + 200),
                                   ci::Color(1, 0.753f, 0.796f), ci::Font("Helvetica", 20));

    }
    else {
        ci::Color8u background_color(0, 0, 0); // black
        ci::gl::clear(background_color);
        unfair_pong_instance.Draw();

        ci::gl::drawStringCentered("Unfair Pong",
                                   vec2(window_length_ / 2.0, 30),
                                   ci::Color("white"), ci::Font("Roboto", 50));

        ci::gl::color(ci::Color("white"));

        ci::gl::draw(sad_emoji_, vec2(window_length_ / 2.0 + 100, 10));

        // start round message (or win screen if someone has won)
        if (unfair_pong_instance.HasTopPlayerWon()) {
            if (game_mode_ == UserVsCPU) {
                ci::gl::drawStringCentered("CPU wins!",
                                           vec2(window_length_ / 2.0, 350),
                                           ci::Color(0.6f, 0.3f, 0.9f), ci::Font("Helvetica", 50));
            }
            else {
                ci::gl::drawStringCentered("CPU 1 wins!",
                                           vec2(window_length_ / 2.0, 350),
                                           ci::Color(0.6f, 0.3f, 0.9f), ci::Font("Helvetica", 50));
            }
        }
        else if (unfair_pong_instance.HasBottomPlayerWon()) {
            if (game_mode_ == UserVsCPU) {
                ci::gl::drawStringCentered("You win!",
                                           vec2(window_length_ / 2.0, 350),
                                           ci::Color(0.6f, 0.3f, 0.9f), ci::Font("Helvetica", 50));
            }
            else {
                ci::gl::drawStringCentered("CPU 2 wins!",
                                           vec2(window_length_ / 2.0, 350),
                                           ci::Color(0.6f, 0.3f, 0.9f), ci::Font("Helvetica", 50));
            }
        }
        else if (unfair_pong_instance.IsDifficultySelected() && !unfair_pong_instance.IsRoundRunning()) {
            ci::gl::drawStringCentered("Press SPACE to start round",
                                       vec2(window_length_ / 2.0, 250),
                                       ci::Color("white"), ci::Font("Helvetica", 40));
        }
    }


}

void UnfairPongApp::mouseDown(ci::app::MouseEvent event) {
    unfair_pong_instance.GetBottomBumper()->MouseMovement(event.getPos());
}

void UnfairPongApp::mouseDrag(ci::app::MouseEvent event) {
    unfair_pong_instance.GetBottomBumper()->MouseMovement(event.getPos());
}

void UnfairPongApp::keyDown(ci::app::KeyEvent event) {
    switch (event.getCode()) {
        case ci::app::KeyEvent::KEY_1: {
            // only allow us to select a difficulty if one hasn't already been selected.
            if (!unfair_pong_instance.IsDifficultySelected()) {
                unfair_pong_instance.SelectDifficulty("easy");
            }
            break;
        }
        case ci::app::KeyEvent::KEY_2: {
            if (!unfair_pong_instance.IsDifficultySelected()) {
                unfair_pong_instance.SelectDifficulty("medium");
            }
            break;
        }
        case ci::app::KeyEvent::KEY_3: {
            if (!unfair_pong_instance.IsDifficultySelected()) {
                unfair_pong_instance.SelectDifficulty("hard");
            }
            break;
        }
        case ci::app::KeyEvent::KEY_4: {
            if (!unfair_pong_instance.IsDifficultySelected()) {
                unfair_pong_instance.SelectDifficulty("unfair");
            }
            break;
        }
        case ci::app::KeyEvent::KEY_SPACE: {
            unfair_pong_instance.StartNewRound();
            break;
        }
        case ci::app::KeyEvent::KEY_z: {
            if (game_mode_ == NotSelected) {
                game_mode_ = UserVsCPU;
                unfair_pong_instance.SelectGameMode(game_mode_);
            }
            break;
        }
        case ci::app::KeyEvent::KEY_x: {
            if (game_mode_ == NotSelected) {
                game_mode_ = CPUVsCPU;
                unfair_pong_instance.SelectGameMode(game_mode_);
            }
            break;
            break;
        }
    }
}

}  // namespace visualizer

}  // namespace naivebayes
