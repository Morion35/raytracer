//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_MATERIALS_HH

#define RAYTRACING_MATERIALS_HH

#include "type.hh"
#include <tuple>

namespace raytracing {
    using texture_values = std::tuple<float, float, float, float, float, color>;

    class Texture_Material {
    public:
        virtual texture_values texture(const p3&) const = 0;
    };

    class Uniform_Material : public Texture_Material {
    public:
        explicit Uniform_Material(color C, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
            : C_(C), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const p3&) const override {
            return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, C_);
        }

    private:
        float Kd_;
        color C_;
        float Ks_;
        float ns_;
        float Kt_;
        float eta_;
    };
}


#endif //RAYTRACING_MATERIALS_HH
