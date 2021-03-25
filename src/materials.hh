//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_MATERIALS_HH

#include "type.hh"
#include <tuple>
namespace raytracing {
    using namespace raytracing;
    class Texture_Material {
    public:
        virtual std::tuple<float, float, float, color> texture(const p3&) const = 0;
    };

    class Uniform_Material : public Texture_Material {
    public:

        Uniform_Material(float Kd, float Ks, float ns, color C) : Ks_(Ks), Kd_(Kd), ns_(ns), C_(C) {};
        Uniform_Material(color c) : C_(c) {};

        std::tuple<float, float, float, color> texture(const p3&) const override {

            return std::tuple(Kd_, Ks_, ns_, C_);
        }

    private:
        float Kd_;
        color C_;
        float Ks_;
        float ns_;
    };
}

#define RAYTRACING_MATERIALS_HH

#endif //RAYTRACING_MATERIALS_HH
