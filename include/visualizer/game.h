#pragma once

#include <core/ball.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <core/cpu_bumper.h>
#include <core/user_bumper.h>


using std::pair;
using std::vector;

namespace unfairpong {

namespace visualizer {


class Game {
 public:

    Game();

    /**
     * Constructor with top_left in canvas, length of game and height of game window
     * This is not the same params as total window which houses other stuff as well
     */
    Game(vec2 top_left, double length, double height);

    /**
     * This method is called by the constructor and deserializes from JSON into member variables
     * According to what was in the JSON file for the selected difficulty
     * After being deserialized it will also start the game by setting is_game_running = true
     */
    void SelectDifficulty(std::string difficulty);

    void Draw();

    void DrawInstructions();

    void DrawScore();

    /**
     * Generates random double between -param and param
     */
    static double Game::GenerateRandomDouble(double absolute_value_limit);

    /**
     * We use this getter for arrow controls.
     */
    UserBumper& GetUserBumper();

    bool IsDifficultySelected();

    bool IsRoundRunning();

    /**
     * This handles mouse movements from a MouseDown event in the cinder app
     */
    void HandleMouseMovement(const vec2& mouse_coords);

    void CheckIfPlayerScored();

    /**
     * These three methods will check for collisions and if there are any, execute them
     */
    void ExecuteBallWallCollision();

    void ExecuteBallUserBumperCollision();

    void ExecuteBallCpuBumperCollision();

    /**
     * This method returns a random vec2 velocity given a bound on the speed, and a specification of whether
     * the y-velocity generated should be positive or negative
     */
    vec2 RandomVelocityGivenSpeed(double speed_desired, bool positive_y_velocity);

    void SetupNewRound();

    void StartNewRound();

    /**
     * This method will update the location of the ball during one time step
     */
    void UpdateBall();

    /**
     * This method will update the location of the user bumper during one time step.
     * The user bumper does have a velocity (if using arrow keys) so we kind of treat it like the ball
     */
    void UpdateUserBumper();

    /**
     * This method will calculate the new movements for the CPU bumper and make them accordingly
     */
    void UpdateCpuBumper();

 private:

    vec2 top_left_corner_;
    double length_;
    double height_;

    double left_wall_;
    double top_wall_;
    double bottom_wall_;
    double right_wall_;


    bool is_difficulty_selected_;
    bool is_round_running_;
    size_t user_score_;
    size_t cpu_score_;


    Ball ball_in_play;
    CpuBumper cpu_bumper_;
    UserBumper user_bumper_;

    float radius_of_ball_;
    size_t points_to_win_;
    cinder::Color color_of_ball_;
    cinder::Color color_of_user_bumper_;
    cinder::Color color_of_cpu_bumper_;
    double starting_ball_velocity_cap_;
    double difficulty_increment_;
    float max_cpu_velocity_;

    double user_bumper_length_;
    double cpu_bumper_length_;

};

}  // namespace visualizer

}  // namespace naivebayes
