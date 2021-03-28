//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_SCENE_HH

#include "objects.hh"
#include "light.hh"
#include "camera.hh"
#include "image.hh"
#include <utility>
#include <vector>
#include <memory>

namespace raytracing {

    class Scene {
    public:

        Scene(std::vector<std::shared_ptr<Object>>  objects, std::vector<std::shared_ptr<Light>>  lights, Camera camera) : objects_(std::move(objects)),
                                                                                         lights_(std::move(lights)),
                                                                                         camera_(camera)
                                                                                         {};
        explicit Scene(Camera camera) : camera_(camera) {};

        Image compute_image(unsigned short width, unsigned short height, unsigned compute_depth, bool smooth = false, bool aliasing = false);

    private:

        std::optional<color> compute_ray(const p3& source, const vec3& ray, unsigned) const;
        std::optional<std::tuple<p3, vec3, float, float, float, color>> cast_ray(const p3& source, const vec3& ray) const;

        std::vector<std::shared_ptr<Object>> objects_;
        std::vector<std::shared_ptr<Light>> lights_;
        Camera camera_;
    };
}

#define RAYTRACING_SCENE_HH

#endif //RAYTRACING_SCENE_HH
