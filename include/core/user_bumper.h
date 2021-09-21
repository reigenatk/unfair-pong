
#ifndef UNFAIR_PONG_USER_BUMPER_H
#define UNFAIR_PONG_USER_BUMPER_H

#include <core/bumper.h>
#include "../../../../include/glm/glm.hpp"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using glm::vec2;

namespace unfairpong {

class UserBumper: public Bumper {
public:
    UserBumper();
    UserBumper(vec2 center_position, double length_of_bumper, cinder::Color color,
               double thickness, float left_wall, float right_wall, double user_smash_rate_);


    /**
     * Use this method when user is moving bumper with mouse- we need to place a limit on
     * where the bumper can be though so there are special cases, not just a simple setter
     */
    void SteerBumperWithMouse(const vec2 &mouse_coords);

    BallType GenerateBallType() override;

    void SmartMovement(vec2 ball_pos, vec2 ball_vel) override;

    void MouseMovement(vec2 mouse_coords) override;

private:

    double user_smash_rate_;
};

}


#endif //UNFAIR_PONG_USER_BUMPER_H
