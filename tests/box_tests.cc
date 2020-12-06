#include <catch2/catch.hpp>
#include <iostream>

#include <visualizer/game.h>
#include <core/ball.h>
#include <core/user_bumper.h>
#include <core/cpu_bumper.h>

using unfairpong::visualizer::Game;
using unfairpong::Ball;
using unfairpong::Bumper;
using unfairpong::UserBumper;
using unfairpong::CpuBumper;
using std::cout;

TEST_CASE("Try collision between Ball and Wall") {
    Game test_instance = Game(vec2(100, 100), 200, 200);
    Ball ball_in_play = Ball(vec2(102.5, 103),
                        vec2(-1, 0), cinder::Color(0, 1, 0), 1, 0.5);
    // should go (102.5, 103), (101.5, 103), (100.5, 103) detect collision, (101.5, 103)

    test_instance.SetBall(ball_in_play);

    test_instance.GetBall().UpdatePositionWithVelocity(vec2(0,0));
    test_instance.ExecuteBallWallCollision();
    Ball& one_tick = test_instance.GetBall();
    REQUIRE(one_tick.GetPosition() == vec2(101.5, 103));
    REQUIRE(one_tick.GetVelocity() == vec2(-1, 0));

    test_instance.GetBall().UpdatePositionWithVelocity(vec2(0,0));
    test_instance.ExecuteBallWallCollision();
    Ball two_tick = test_instance.GetBall();
    REQUIRE(two_tick.GetPosition() == vec2(100.5, 103));
    REQUIRE(one_tick.GetVelocity() == vec2(1, 0));

    test_instance.GetBall().UpdatePositionWithVelocity(vec2(0,0));
    test_instance.ExecuteBallWallCollision();
    Ball three_tick = test_instance.GetBall();
    REQUIRE(three_tick.GetPosition() == vec2(101.5, 103));
    REQUIRE(one_tick.GetVelocity() == vec2(1, 0));
}

TEST_CASE("Test collision between ball and bumpers") {

    SECTION("Top Bumper collision with CPU bumper") {
        Game test_instance = Game(vec2(100, 100), 200, 200);
        Ball ball_in_play = Ball(vec2(150, 138.5),
                                 vec2(0, -4), cinder::Color(0, 1, 0), 1, 0.5);
        Bumper* cpu_bumper = new CpuBumper(vec2(150, 100), 50,
                                           cinder::Color(0, 1, 0), 30, 30, 100, 300,
                                           0.1, 0.2, 0.25, 0.3, 0.4);
        test_instance.SetTopBumper(cpu_bumper);
        test_instance.SetBall(ball_in_play);

        // ball goes (150, 138.5), (150, 134.5), (150, 130.5) detect collision, then depending on if its a smash
        // ball or not, goes outwards with a velocity of either current + smash increase or current + diff increment
        test_instance.GetBall().UpdatePositionWithVelocity(vec2(0, 0));
        test_instance.GetBall().UpdatePositionWithVelocity(vec2(0, 0));

        REQUIRE(test_instance.GetBall().GetPosition() == vec2(150, 130.5));

        test_instance.GetBall().CollideWithTopBumper(test_instance.GetTopBumper(), 100, 300, 100, 300);
        // since diff increment is 0.5 and smassh velocity increment is 10, we expect the ball to
        // have either velocity of (current + 0.5) or (current + 10)


        double len = length(test_instance.GetBall().GetVelocity());
        bool correct_velocity = false;
        if (len == Approx(4.5).margin(0.02) || len == Approx(15).margin(0.02)) {
            correct_velocity = true;
        }
        REQUIRE(correct_velocity);
        // we can't say much more about the velocity since it could be any of the 5 types of balls.
    }

    SECTION("Bottom Bumper collision with User bumper") {
        Game test_instance = Game(vec2(100, 100), 200, 200);
        Ball ball_in_play = Ball(vec2(150, 261.5),
                                 vec2(0, 4), cinder::Color(0, 1, 0), 1, 1);
        Bumper* user_bumper = new UserBumper(vec2(150, 300),
                                             30, cinder::Color(0, 1, 0),
                                             30, 100, 300, 0.4);
        test_instance.SetBottomBumper(user_bumper);
        test_instance.SetBall(ball_in_play);

        // ball goes (150, 261.5), (150, 265.5), (150, 269.5) detect collision, then depending on if its a smash
        // ball or not, goes outwards with a velocity of either current + smash increase or current + diff increment

        test_instance.GetBall().UpdatePositionWithVelocity(vec2(0, 0));
        test_instance.GetBall().UpdatePositionWithVelocity(vec2(0, 0));

        REQUIRE(test_instance.GetBall().GetPosition() == vec2(150, 269.5));

        test_instance.GetBall().CollideWithBottomBumper(test_instance.GetBottomBumper(), 100, 300, 100, 300);
        // since diff increment is 1 and smassh velocity increment is 10, we expect the ball to
        // have either velocity of (4 + 1) or (4 + 10)


        double len = length(test_instance.GetBall().GetVelocity());
        bool correct_velocity = false;
        cout << len;
        if (len == Approx(5).margin(0.02) || len == Approx(14).margin(0.02)) {
            correct_velocity = true;
        }
        REQUIRE(correct_velocity);
    }
}

TEST_CASE("Check if the game is detecting scoring") {
    SECTION("Top Player Scores") {
        Game test_instance = Game(vec2(100, 100), 200, 200);
        Ball ball_in_play = Ball(vec2(150, 291.5),
                                 vec2(0, 4), cinder::Color(0, 1, 0), 1, 1);
        test_instance.SetBall(ball_in_play);
        test_instance.GetBall().UpdatePositionWithVelocity(vec2(0, 0));
        test_instance.GetBall().UpdatePositionWithVelocity(vec2(0, 0));
        REQUIRE(test_instance.GetBall().GetPosition() == vec2(150, 299.5));

        test_instance.SetTopPlayerScore(4);
        test_instance.SetPointsToWin(5);

        // next tick scores
        test_instance.GetBall().UpdatePositionWithVelocity(vec2(0, 0));
        test_instance.CheckIfPlayerScored();

        REQUIRE(test_instance.GetTopPlayerScore() == 5);
        REQUIRE(test_instance.HasTopPlayerWon());
    }
    SECTION("Bottom Player Scores") {
        Game test_instance = Game(vec2(100, 100), 200, 200);
        Ball ball_in_play = Ball(vec2(150, 104.5),
                                 vec2(0, -4), cinder::Color(0, 1, 0), 1, 1);
        test_instance.SetBall(ball_in_play);
        test_instance.GetBall().UpdatePositionWithVelocity(vec2(0, 0));
        REQUIRE(test_instance.GetBall().GetPosition() == vec2(150, 100.5));

        test_instance.SetBottomPlayerScore(4);
        test_instance.SetPointsToWin(5);

        // next tick scores
        test_instance.GetBall().UpdatePositionWithVelocity(vec2(0, 0));
        test_instance.CheckIfPlayerScored();

        REQUIRE(test_instance.GetBottomPlayerScore() == 5);
        REQUIRE(test_instance.HasBottomPlayerWon());
    }


}

