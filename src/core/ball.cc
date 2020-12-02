#include <visualizer/game.h>
#include <core/ball.h>
#include <iostream>
#include <algorithm>

using std::cout;
using unfairpong::visualizer::Game;

namespace unfairpong {

Ball::Ball() {

}

Ball::Ball(vec2 starting_position, vec2 starting_velocity, cinder::Color color,
           double radius, double user_smash_rate, double cpu_smash_rate, double cpu_dizzy_rate,
           double cpu_monkey_rate, double cpu_brittle_rate, double difficulty_increment) {
    position_ = starting_position;
    velocity_ = starting_velocity;
    color_ = color;
    radius_ = radius;
    user_smash_rate_ = user_smash_rate;
    cpu_smash_rate_ = cpu_smash_rate;
    difficulty_increment_ = difficulty_increment;

    max_ball_velocity = 20; // can tweak this but I found 20 to work ok
    smash_velocity_increase_ = 10; // just how much faster does the ball get on a smash hit
    cpu_dizzy_rate_ = cpu_dizzy_rate;
    cpu_monkey_rate_ = cpu_monkey_rate;
    cpu_brittle_rate_ = cpu_brittle_rate;

    effect_image_ = nullptr;
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

void Ball::ChangeIntoRandomColor() {
    double r = Game::GenerateRandomDoubleBetween(0, 1);
    double g = Game::GenerateRandomDoubleBetween(0, 1);
    double b = Game::GenerateRandomDoubleBetween(0, 1);

    color_ = cinder::Color((float) r, (float) g, (float) b);
}

void Ball::ResetForNewRound(vec2 new_position, vec2 new_velocity) {
    position_ = new_position;
    velocity_ = new_velocity;
    ResetAllEffects();
}

void Ball::ResetAllEffects() {
    is_smash_ball_ = false;
    is_dizzy_ball_ = false;
    is_monkey_ball_ = false;
    is_brittle_ball_ = false;
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

void Ball::CollideWithUserBumper(UserBumper& user_bumper, float left_wall_x, float right_wall_x, float top_wall_y, float bottom_wall_y) {

    // first check if its even  colliding
    if (abs(position_.y - bottom_wall_y) < user_bumper.GetBumperThickness() + (float) (radius_ / 2.0) &&
        abs(position_.x - user_bumper.GetBumperCenter().x) < (float) (user_bumper.GetBumperLength() / 2.0)) {
        // is colliding
    }
    else {
        // if not
        return;
    }

    // if the ball colliding was a brittle ball, then we need to "break off" a part of the user bumper.
    if (is_brittle_ball_) {
        if (position_.x >= user_bumper.GetBumperCenter().x) {
            // if the ball landed on the right side of the bumper

            vec2 new_bumper_left = user_bumper.GetBumperCenter() + vec2(-user_bumper.GetBumperLength() / 2.0, 0);
            vec2 new_bumper_right = vec2(position_.x, user_bumper.GetBumperCenter().y);
            double amount_to_break = user_bumper.GetBumperCenter().x + (user_bumper.GetBumperLength() / 2.0) - position_.x;
            user_bumper.SetBumperLength(user_bumper.GetBumperLength() - amount_to_break);

            vec2 new_center = new_bumper_left + new_bumper_right;
            new_center.x /= 2.0;
            new_center.y /= 2.0;
            user_bumper.SetBumperCenter(new_center);
        }
        else {
            // if the ball landed on the left side of the bumper
            vec2 new_bumper_left = vec2(position_.x, user_bumper.GetBumperCenter().y);
            vec2 new_bumper_right = user_bumper.GetBumperCenter() + vec2(user_bumper.GetBumperLength() / 2.0, 0);
            double amount_to_break = position_.x - (user_bumper.GetBumperCenter().x - (user_bumper.GetBumperLength() / 2.0));
            user_bumper.SetBumperLength(user_bumper.GetBumperLength() - amount_to_break);

            vec2 new_center = new_bumper_left + new_bumper_right;
            new_center.x /= 2.0;
            new_center.y /= 2.0;
            user_bumper.SetBumperCenter(new_center);
        }
    }

    if (Game::RollChance(user_smash_rate_)) {
        // if smash ball

        double target_x_location = Game::GenerateRandomDoubleBetween(left_wall_x, right_wall_x);
        double velocity_of_ball = fmin(max_ball_velocity, length(velocity_) + smash_velocity_increase_);
        velocity_ = VelocityGivenTargetAndSpeed(vec2(target_x_location, top_wall_y), velocity_of_ball);
        ResetAllEffects();
        is_smash_ball_ = true;
    }
    else {
        // if last ball was smash ball, slow down the velocity by however much the previous smash increased it
        // kind of like how receiving a tennis serve will slow the ball down to a slower speed
        double new_velocity;
        if (is_smash_ball_) {
            new_velocity = fmin(max_ball_velocity, length(velocity_) + difficulty_increment_ - smash_velocity_increase_);
        } else {
            new_velocity = fmin(max_ball_velocity, length(velocity_) + difficulty_increment_);
        }
        velocity_ = Game::RandomVelocityGivenSpeed(new_velocity,
                                                         false);
        ResetAllEffects();
    }

}

void Ball::CollideWithCpuBumper(CpuBumper& cpu_bumper, float left_wall_x, float right_wall_x, float top_wall_y, float bottom_wall_y) {
    // important note about effects, the ball can only have one effect at a time.
    // so there is no such thing as a dizzy smash ball for example- I've set up the logic here
    // so that its impossible to get both effects

    // also as of now I am only assigning CPU the power to issue out monkey, dizzy, and brittle balls

    if (abs(position_.y - top_wall_y) < cpu_bumper.GetBumperThickness() + (float) (radius_ / 2.0) &&
        abs(position_.x - cpu_bumper.GetBumperCenter().x) < (float) (cpu_bumper.GetBumperLength() / 2.0)) {
        // is colliding
    }
    else {
        return;
    }

    if (Game::RollChance(cpu_smash_rate_)) {
        // if smash ball
        double target_x_location = Game::GenerateRandomDoubleBetween(left_wall_x, right_wall_x);
        double velocity_of_ball = fmin(max_ball_velocity, length(velocity_) + smash_velocity_increase_);
        velocity_ = VelocityGivenTargetAndSpeed(vec2(target_x_location, bottom_wall_y), velocity_of_ball);
        is_smash_ball_ = true;
    }
    else {

        double new_velocity;
        if (is_smash_ball_) {
            // if last ball was smash ball
            new_velocity = fmin(max_ball_velocity, length(velocity_) + difficulty_increment_ - smash_velocity_increase_);
        } else {
            new_velocity = fmin(max_ball_velocity, length(velocity_) + difficulty_increment_);
        }

        if (Game::RollChance(cpu_dizzy_rate_)) {
            // if cpu gets dizzy ball
            ResetAllEffects();
            is_dizzy_ball_ = true;
        }
        else if (Game::RollChance(cpu_monkey_rate_)) {
            // if cpu gets monkey ball
            ResetAllEffects();
            is_monkey_ball_ = true;
        }
        else if (Game::RollChance(cpu_brittle_rate_)) {
            ResetAllEffects();
            is_brittle_ball_ = true;
        }
        else {
            // if no effect
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
    if (is_smash_ball_) {
        ci::gl::color(ci::Color("white"));
        if (effect_image_ == nullptr) {
            effect_image_ = cinder::gl::Texture2d::create
                    (cinder::loadImage("../../../data/fire.png"));
        }
        ci::gl::draw(effect_image_, position_);
    }
    else if (is_dizzy_ball_) {
        ci::gl::color(ci::Color("white"));
        if (effect_image_ == nullptr) {
            effect_image_ = cinder::gl::Texture2d::create
                    (cinder::loadImage("../../../data/dizzy.png"));
        }
        ci::gl::draw(effect_image_, position_);
    }
    else if (is_monkey_ball_) {
        ci::gl::color(ci::Color("white"));
        if (effect_image_ == nullptr) {
            effect_image_ = cinder::gl::Texture2d::create
                    (cinder::loadImage("../../../data/monkey.png"));
        }
        ci::gl::draw(effect_image_, position_);
    }
    else if (is_brittle_ball_) {
        ci::gl::color(ci::Color("white"));
        if (effect_image_ == nullptr) {
            effect_image_ = cinder::gl::Texture2d::create
                    (cinder::loadImage("../../../data/brittle.png"));
        }
        ci::gl::draw(effect_image_, position_);
    }
    ci::gl::color(color_);
    ci::gl::drawSolidCircle(position_, (float) radius_);
}

void Ball::UpdatePositionWithVelocity(vec2 farther_user_corner) {
    position_ += velocity_;

    double dizziness = 1; // can adjust this, basically how much the ball wavers around
    if (is_dizzy_ball_) {
        velocity_ += vec2(Game::GenerateRandomDouble(dizziness), 0);
    }

    if (is_monkey_ball_) {
        velocity_ = VelocityGivenTargetAndSpeed(farther_user_corner, length(velocity_));
    }
}

}