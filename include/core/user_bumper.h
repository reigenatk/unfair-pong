
#ifndef UNFAIR_PONG_USER_BUMPER_H
#define UNFAIR_PONG_USER_BUMPER_H

#include <cinder/Color.h>
#include "../../../../include/glm/glm.hpp"
using glm::vec2;

namespace unfairpong {

    class UserBumper {
    public:
        UserBumper();
        UserBumper(vec2 center_position, double length_of_bumper, cinder::Color color,
                   double thickness, float left_wall, float right_wall);
        void Draw() const;
        void ExecuteTimeStep();
        void MoveBumperLeft();
        void MoveBumperRight();
        void StopBumper();
        void SetBumperCenter(const vec2 &mouse_coords);

        double GetBumperThickness();
        vec2& GetBumperCenter();
        double GetBumperLength();

    private:
        vec2 center_position_;
        float horizontal_velocity_of_bumper;
        double length_of_bumper_;
        cinder::Color bumper_color_;
        double thickness_of_bumper_;
        float left_wall_;
        float right_wall_;
    };

}


#endif //UNFAIR_PONG_USER_BUMPER_H
