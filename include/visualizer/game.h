#pragma once

#include <core/ball.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <core/bumper.h>
#include <core/gamemode.h>


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
     */
    void SelectDifficulty(std::string difficulty);

    void SelectGameMode(GameMode g);

    void Draw();

    void DrawInstructions() const;

    void DrawScore() const;

    double GetLeftWallX() const;

    double GetRightWallX() const;

    double GetTopWallY() const;

    double GetBottomWallY() const;

    Ball& GetBall();

    // for testing
    void SetBall(Ball test_ball);

    void SetTopBumper(Bumper* test_bumper);

    void SetBottomBumper(Bumper* test_bumper);

    void SetTopPlayerScore(size_t score);

    void SetBottomPlayerScore(size_t score);

    void SetPointsToWin(size_t points);

    /**
     * Generates random double between -param and param
     */
    static double Game::GenerateRandomDouble(double absolute_value_limit);

    static double Game::GenerateRandomDoubleBetween(double lb, double ub);

    /**
    * Returns true or false. Use this to determine chances of obtaining certain powerups
    * @param chance- the chance of returning true
    */
    static bool RollChance(double chance);

    /**
     * This method returns a random vec2 velocity given a bound on the speed, and a specification of whether
     * the y-velocity generated should be positive or negative
     */
    static vec2 RandomVelocityGivenSpeed(double speed_desired, bool positive_y_velocity);

    Bumper* GetTopBumper();

    Bumper* GetBottomBumper();

    size_t GetTopPlayerScore() const;

    size_t GetBottomPlayerScore() const;

    bool IsDifficultySelected() const;

    bool IsRoundRunning() const;

    bool HasTopPlayerWon() const;

    bool HasBottomPlayerWon() const;

    /**
     * Looking at the positions of the ball and the walls, determine whether the player has scored
     * and if they have, stop the current round and award the winning player a point
     */
    void CheckIfPlayerScored();

    /**
     * This method is called every time step, what it does is it calls the position update methods first
     * and then the collision execution functions after
     */
    void UpdateAll();

    /**
     * These three methods will check for collisions and if there are any, execute them
     */
    void ExecuteBallWallCollision();

    /**
     * Place the ball back in the center, along with bumpers. Assign the ball a new random starting velocity
     */
    void SetupNewRound();

    /**
     * Set isgamerunning to true which will trigger update in the cinder app
     */
    void StartNewRound();

    /**
     * This method will update the location of the ball during one time step
     */
    void UpdateBall();

    void UpdateBumpers();

 private:

    vec2 top_left_corner_;
    double length_;
    double height_;

    // these variables deal with game state
    bool is_difficulty_selected_;
    bool is_round_running_;
    size_t bottom_player_score_;
    size_t top_player_score_;


    Ball ball_in_play;
    // need to store pointers since Bumper itself is an abstract class with abstract methods
    // so cant have an instance of it
    Bumper* top_bumper;
    Bumper* bottom_bumper;

    // points_to_win makes sense to be in this class
    size_t points_to_win_;

    // I also thought these two made sense to be in the Game class since they're only ever used to
    // initialize ball velocities in the SetupNewRound() method and in the constructor
    double starting_ball_velocity_floor_;
    double starting_ball_velocity_cap_;

    // depending on what game mode the user selects, we will
    // initialize either 2 CPU bumpers or 1 CPU bumper for top and 1 User bumper for bottom
    GameMode game_mode_;

};

}  // namespace visualizer

}  // namespace naivebayes
