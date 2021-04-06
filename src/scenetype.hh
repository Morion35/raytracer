//
// Created by simon on 4/5/21.
//

#ifndef RAYTRACING_SCENETYPE_HH

#define RAYTRACING_SCENETYPE_HH

#include "image.hh"
#include "objects.hh"

#include <optional>
#include <tuple>

namespace raytracing {
    class SceneType {
    public:
        virtual Image compute_image(unsigned short width, unsigned short height, unsigned compute_depth, bool aliasing) = 0;

        virtual std::optional<color> compute_ray(const p3& source, const vec3& ray, unsigned) const = 0;
        virtual std::optional<std::tuple<p3, vec3, float, float, float, float, float, color>> cast_ray(const p3& source, const vec3& ray) const = 0;
        virtual std::vector<std::tuple<color, double>> light_passing_objects(const p3& begin, const p3& end) const = 0;

    };
}

#endif //RAYTRACING_SCENETYPE_HH
