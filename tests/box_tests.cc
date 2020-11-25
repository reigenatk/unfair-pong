#include <catch2/catch.hpp>
#include <iostream>

#include <visualizer/game.h>
#include <core/ball.h>

using unfairpong::visualizer::Game;

TEST_CASE("Try collision between Ball and Wall") {
    Game test_instance = Game(vec2(100, 100), 200, 200);

}


