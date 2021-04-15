//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_SCENE_HH

#define RAYTRACING_SCENE_HH

#include "objects.hh"
#include "camera.hh"
#include "image.hh"
#include <utility>
#include <vector>
#include <memory>
#include "light.hh"
#include "scenetype.hh"

namespace raytracing {

    class Scene : public SceneType {
    public:

        Scene(std::vector<std::shared_ptr<Object>>  objects, std::vector<std::shared_ptr<Light>>  lights,
              Camera camera, double ambiant=0.05) :
              objects_(std::move(objects)), lights_(std::move(lights)),
              camera_(std::move(camera)), ambiant_(ambiant) {};

        explicit Scene(Camera camera) : camera_(std::move(camera)) {};

        Image compute_image(unsigned short width, unsigned short height, unsigned compute_depth, bool antialiasing, bool dynamic_dof) override;


        std::optional<color> compute_ray(const p3& source, const vec3& ray, unsigned) const override;
        std::optional<std::tuple<p3, vec3, float, float, float, float, float, color>> cast_ray(const p3& source, const vec3& ray) const override;

        std::vector<std::tuple<color, double>> light_passing_objects(const p3& begin, const p3& end) const override;

    private:

        static double fresnel(const raytracing::vec3 &I, const raytracing::vec3 &norm, float eta);

        static std::optional<raytracing::vec3> refract(const raytracing::vec3 &I, raytracing::vec3 norm, float eta);

        std::vector<std::shared_ptr<Object>> objects_;
        std::vector<std::shared_ptr<Light>> lights_;
        const Camera camera_;

        double ambiant_;
    };
}


#endif //RAYTRACING_SCENE_HH
