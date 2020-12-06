#include <visualizer/game.h>
#include <core/ball.h>
#include <iostream>
#include <algorithm>

using std::cout;
using unfairpong::visualizer::Game;

namespace unfairpong {

Ball::Ball() {

}

Ball::Ball(vec2 starting_position, vec2 starting_velocity, cinder::Color color, double radius, double difficulty_increment) {
    position_ = starting_position;
    velocity_ = starting_velocity;
    color_ = color;
    radius_ = radius;

    max_ball_velocity = 20; // can tweak this but I found 20 to work ok
    smash_velocity_increase_ = 10; // just how much faster does the ball get on a smash hit
    difficulty_increment_ = difficulty_increment;

    ResetAllEffects();
}

vec2& Ball::GetPosition() {
    return position_;
}

vec2& Ball::GetVelocity() {
    return velocity_;
}

double Ball::GetRadius() const {
    return radius_;
}

cinder::Color Ball::GetColor() const {
    return color_;
}

void Ball::ResetForNewRound(vec2 new_position, vec2 new_velocity) {
    position_ = new_position;
    velocity_ = new_velocity;
    ResetAllEffects();
}

void Ball::ResetAllEffects() {
    type_of_ball_ = Normal;

    // set these two params back to normal so that next time we roll a random ball they will be correct
    frames_elapsed_ = 0;
    frames_until_random_ = 10; // can change this- basically dictates how often a random jerk occurs (when we roll a random ball)

    // im using nullpointer here to indicate when there is NO effect image (aka a normal ball)
    effect_image_ = nullptr;
}

vec2 Ball::VelocityGivenTargetAndSpeed(vec2 target_pos, double desired_velocity_of_ball) {
    double xdiff = target_pos.x - position_.x;
    double ydiff = target_pos.y - position_.y;

    vec2 ret = vec2(xdiff, ydiff);
    double len = length(ret);
    double scale_factor = desired_velocity_of_ball / len;
    ret.x *= (float) scale_factor;
    ret.y *= (float) scale_factor;

    return ret;
}

void Ball::CollideWithBottomBumper(Bumper* bottom_bumper, float left_wall_x, float right_wall_x, float top_wall_y, float bottom_wall_y) {

    // first check if its even  colliding
    if (abs(position_.y - bottom_wall_y) < bottom_bumper->GetBumperThickness() + (float) (radius_ / 2.0) &&
        abs(position_.x - bottom_bumper->GetBumperCenter().x) < (float) (bottom_bumper->GetBumperLength() / 2.0)) {
        // is colliding
    }
    else {
        // if not
        return;
    }

    // if the ball colliding was a brittle ball, then we need to "break off" a part of the bumper.
    if (type_of_ball_ == Brittle && velocity_.y > 0) {
        bottom_bumper->ExecuteBrittleCollision(position_);
    }

    BallType next_ball_type = bottom_bumper->GenerateBallType();

    if (next_ball_type == Smash) {
        // if smash ball

        double target_x_location = Game::GenerateRandomDoubleBetween(left_wall_x, right_wall_x);
        double velocity_of_ball = fmin(max_ball_velocity, length(velocity_) + smash_velocity_increase_);
        velocity_ = VelocityGivenTargetAndSpeed(vec2(target_x_location, top_wall_y), velocity_of_ball);
        ResetAllEffects();
        type_of_ball_ = Smash;
    }
    else {
        double new_velocity;
        if (type_of_ball_ == Smash) {
            // if last ball was smash ball
            new_velocity = fmin(max_ball_velocity, length(velocity_) + difficulty_increment_ - smash_velocity_increase_);
        } else {
            new_velocity = fmin(max_ball_velocity, length(velocity_) + difficulty_increment_);
        }

        if (next_ball_type == Dizzy) {
            // if cpu gets dizzy ball
            ResetAllEffects();
            type_of_ball_ = Dizzy;
        }
        else if (next_ball_type == Monkey) {
            // if cpu gets monkey ball
            ResetAllEffects();
            type_of_ball_ = Monkey;
        }
        else if (next_ball_type == Brittle) {
            ResetAllEffects();
            type_of_ball_ = Brittle;
        }
        else if (next_ball_type == Random) {
            ResetAllEffects();
            type_of_ball_ = Random;
        }
        else {
            // will set type of ball to Normal
            ResetAllEffects();
        }
        // generate a velocity that is going the other way (negative y velocity)
        velocity_ = Game::RandomVelocityGivenSpeed(new_velocity,
                                                   false);

    }

}

void Ball::CollideWithTopBumper(Bumper* top_bumper, float left_wall_x, float right_wall_x, float top_wall_y, float bottom_wall_y) {
    // important note about effects, the ball can only have one effect at a time.
    // so there is no such thing as a dizzy smash ball for example- I've set up the logic here
    // so that its impossible to get both effects

    if (abs(position_.y - top_wall_y) < top_bumper->GetBumperThickness() + (float) (radius_ / 2.0) &&
        abs(position_.x - top_bumper->GetBumperCenter().x) < (float) (top_bumper->GetBumperLength() / 2.0)) {
        // is colliding
    }
    else {
        return;
    }

    if (type_of_ball_ == Brittle && velocity_.y < 0) {
        top_bumper->ExecuteBrittleCollision(position_);
    }

    BallType next_ball_type = top_bumper->GenerateBallType();

    if (next_ball_type == Smash) {
        // if smash ball
        double target_x_location = Game::GenerateRandomDoubleBetween(left_wall_x, right_wall_x);
        double velocity_of_ball = fmin(max_ball_velocity, length(velocity_) + smash_velocity_increase_);
        velocity_ = VelocityGivenTargetAndSpeed(vec2(target_x_location, bottom_wall_y), velocity_of_ball);
        type_of_ball_ = Smash;
    }
    else {

        double new_velocity;
        if (type_of_ball_ == Smash) {
            // if last ball was smash ball
            new_velocity = fmin(max_ball_velocity, length(velocity_) + difficulty_increment_ - smash_velocity_increase_);
        } else {
            new_velocity = fmin(max_ball_velocity, length(velocity_) + difficulty_increment_);
        }

        if (next_ball_type == Dizzy) {
            // if cpu gets dizzy ball
            ResetAllEffects();
            type_of_ball_ = Dizzy;
        }
        else if (next_ball_type == Monkey) {
            // if cpu gets monkey ball
            ResetAllEffects();
            type_of_ball_ = Monkey;
        }
        else if (next_ball_type == Brittle) {
            ResetAllEffects();
            type_of_ball_ = Brittle;
        }
        else if (next_ball_type == Random) {
            ResetAllEffects();
            type_of_ball_ = Random;
        }
        else {
            // will set type of ball to Normal
            ResetAllEffects();
        }
        velocity_ = Game::RandomVelocityGivenSpeed(new_velocity,
                                                   true);

    }
}

void Ball::Draw() {
    // I tried to make it so that we don't have to load the image in each frame as that would be very taxing
    // on the efficiency- so I set a member variable called effect_image_
    // to nullptr after each collision and load in the necessary image once
    // on the first frame
    if (type_of_ball_ == Smash) {
        ci::gl::color(ci::Color("white"));
        if (effect_image_ == nullptr) {
            effect_image_ = cinder::gl::Texture2d::create
                    (cinder::loadImage("../../../data/fire.png"));
        }
        ci::gl::draw(effect_image_, position_);
    }
    else if (type_of_ball_ == Dizzy) {
        ci::gl::color(ci::Color("white"));
        if (effect_image_ == nullptr) {
            effect_image_ = cinder::gl::Texture2d::create
                    (cinder::loadImage("../../../data/dizzy.png"));
        }
        ci::gl::draw(effect_image_, position_);
    }
    else if (type_of_ball_ == Monkey) {
        ci::gl::color(ci::Color("white"));
        if (effect_image_ == nullptr) {
            effect_image_ = cinder::gl::Texture2d::create
                    (cinder::loadImage("../../../data/monkey.png"));
        }
        ci::gl::draw(effect_image_, position_);
    }
    else if (type_of_ball_ == Brittle) {
        ci::gl::color(ci::Color("white"));
        if (effect_image_ == nullptr) {
            effect_image_ = cinder::gl::Texture2d::create
                    (cinder::loadImage("../../../data/brittle.png"));
        }
        ci::gl::draw(effect_image_, position_);
    }
    else if (type_of_ball_ == Random) {
        ci::gl::color(ci::Color("white"));
        if (effect_image_ == nullptr) {
            effect_image_ = cinder::gl::Texture2d::create
                    (cinder::loadImage("../../../data/random.png"));
        }
        ci::gl::draw(effect_image_, position_);
    }
    ci::gl::color(color_);
    ci::gl::drawSolidCircle(position_, (float) radius_);
}

void Ball::UpdatePositionWithVelocity(vec2 farther_user_corner) {
    frames_elapsed_++;
    position_ += velocity_;

    double dizziness = 1; // can adjust this, basically how much the ball wavers around
    if (type_of_ball_ == Dizzy) {
        velocity_ += vec2(Game::GenerateRandomDouble(dizziness), 0);
    }
    if (type_of_ball_ == Monkey) {
        velocity_ = VelocityGivenTargetAndSpeed(farther_user_corner, length(velocity_));
    }
    if (type_of_ball_ == Random && frames_elapsed_ == frames_until_random_) {
        // do a "jerk" of motion

        // figure out what the y velocity is, we don't want to change that
        // in other words ball should always be heading towards other player
        bool positive_y_velocity;
        if (velocity_.y < 0) {
            positive_y_velocity = false;
        }
        else {
            positive_y_velocity = true;
        }

        // add a little bit of speed with each jerk of motion
        double speed_increment_per_jerk = 0.4;
        vec2 new_velocity = Game::RandomVelocityGivenSpeed(length(velocity_) + speed_increment_per_jerk, positive_y_velocity);

        velocity_ = new_velocity;

        // for now I'm making the number of frames elapsed until a random jerk occurs, go up by 2 each time
        // so it starts 50, 52, 54, ...
        // can change this of course

        size_t jerk_increment = 2;
        frames_until_random_ += jerk_increment;
        frames_elapsed_ = 0;
    }

}

}