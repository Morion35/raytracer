//
// Created by pakito on 4/13/21.
//

#ifndef RAYTRACING_TRANSFORMER_HH
#define RAYTRACING_TRANSFORMER_HH

namespace raytracing {
    struct Transformer {
        Transformer() = default;

        Transformer(const vec3& forward, const vec3& right, const vec3& up) :
                x_(right.u, up.u, forward.u),
                y_(right.v, up.v, forward.v),
                z_(right.w, up.w, forward.w),
                forward_(forward), right_(right), up_(up)
        {}

        vec3 operator()(const vec3& v) const {
            return to_basis(v);
        }

        vec3 to_basis(const vec3& v) const {
            return vec3(x_.dot(v), y_.dot(v), z_.dot(v));
        }

        vec3 from_basis(const vec3& v) const {
            return vec3(right_.dot(v), up_.dot(v), forward_.dot(v));
        }

        vec3 x_;
        vec3 y_;
        vec3 z_;

        vec3 forward_;
        vec3 right_;
        vec3 up_;
    };
}

#endif //RAYTRACING_TRANSFORMER_HH
