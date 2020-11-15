#ifndef UNFAIR_PONG_CPU_BUMPER_H
#define UNFAIR_PONG_CPU_BUMPER_H

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "../../../../include/glm/glm.hpp"
using glm::vec2;

namespace unfairpong {
    class CpuBumper {
    public:
        CpuBumper();
        CpuBumper(vec2 center_position, double length_of_bumper, cinder::Color color, double thickness);
        void Draw() const;

    private:
        vec2 center_position_;
        double length_of_bumper_;
        cinder::Color bumper_color_;
        double thickness_of_bumper_;

    };

}








#endif //UNFAIR_PONG_CPU_BUMPER_H
