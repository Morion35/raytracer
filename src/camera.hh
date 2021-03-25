//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_CAMERA_HH

#include "type.hh"
#include <vector>

namespace raytracing {
    class Camera {
    public:

        Camera(p3 origin, p3 vue, vec3 sky, float ax, float ay, p3 dz) : C_(origin), P_(vue), sky_(sky), ax_(ax), ay_(ay), dz_(dz) {};

        std::vector<std::vector<vec3>> get_rays(unsigned short width, unsigned short height) const;

        p3 source() const { return C_; }

    private:
        p3 C_;
        p3 P_;
        vec3 sky_;
        float ax_;
        float ay_;
        p3 dz_;
    };
}

#define RAYTRACING_CAMERA_HH

#endif //RAYTRACING_CAMERA_HH
