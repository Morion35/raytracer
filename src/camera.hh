//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_CAMERA_HH

#define RAYTRACING_CAMERA_HH

#include "type.hh"
#include <vector>

namespace raytracing {
    class Camera {
    public:

        Camera(p3 origin, p3 viewpoint, vec3 sky, double ax, double ay, double dz, double e = 0.3, double d = 0.1);

        std::vector<std::vector<std::vector<p3>>> get_pixels(unsigned short width, unsigned short height, bool aliasing = false) const;

        std::vector<p3> source() const { return sources_; };

    private:
        vec3 transform(const vec3& v) const { return vec3(x_.dot(v), y_.dot(v), z_.dot(v)); }
        p3 C_;
        p3 P_;
        std::vector<p3> sources_;
        vec3 sky_;
        double ax_;
        double ay_;
        double dz_;
        double e_;
        double d_;
        vec3 n_;
        vec3 right_;
        vec3 up_;
        vec3 x_;
        vec3 y_;
        vec3 z_;
    };
}


#endif //RAYTRACING_CAMERA_HH
