//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_CAMERA_HH

#define RAYTRACING_CAMERA_HH

#include "type.hh"
#include <vector>
#include <random>

namespace raytracing {
    struct Transformer : std::unary_function<const vec3&, vec3> {
        Transformer(const vec3& forward, const vec3& right, const vec3& up) :
                x_(right.u, up.u, forward.u),
                y_(right.v, up.v, forward.v),
                z_(right.w, up.w, forward.w)
        {}
        vec3 operator()(const vec3& v) const {
            return vec3(x_.dot(v), y_.dot(v), z_.dot(v));
        }
        const vec3 x_;
        const vec3 y_;
        const vec3 z_;
    };

    class Camera {
    public:

        Camera(p3 origin, p3 viewpoint, vec3 sky, double ax, double ay, double dz, double e = 0.3, double d = 0.1);

        std::vector<std::vector<std::vector<p3>>> get_pixels(unsigned short width, unsigned short height, bool aliasing = false) const;

        std::vector<p3> source() const { return sources_; };

        double get_focal() const { return d_; }
        vec3 transform(const vec3& v) const { return transform_(v); }
        Transformer transformer() const { return transform_; }

    private:
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
        Transformer transform_;
        std::uniform_real_distribution<double> unif_;
        std::default_random_engine re_;
    };
}


#endif //RAYTRACING_CAMERA_HH
