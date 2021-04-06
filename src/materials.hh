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
        virtual texture_values texture(const p3&, const vec3& n) const = 0;
    };

    class Uniform_Material : public Texture_Material {
    public:
        explicit Uniform_Material(color C, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
            : C_(C), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const p3&, const vec3& n) const override {
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

    class Plane_Paved_Material : public Texture_Material {
    public:
        explicit Plane_Paved_Material(color C1, color C2, double e, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
                : C1_(C1), C2_(C2), e_(e), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const p3& p, const vec3& n) const override {
            double sines = std::sin(M_PIf32 / e_ * p.x) * std::sin(M_PIf32 / e_ * p.y) * std::sin(M_PIf32 / e_ * p.z);
            return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, sines > 0 ? C1_ : C2_);
        }

    private:
        float Kd_;
        double e_;
        color C1_;
        color C2_;
        float Ks_;
        float ns_;
        float Kt_;
        float eta_;
    };

    class Spherical_Paved_Material : public Texture_Material {
    public:
        explicit Spherical_Paved_Material(color C1, color C2, double e, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
                : C1_(C1), C2_(C2), e_(e), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const p3& p, const vec3& n) const override {
            double phi = std::atan2(n.w, n.u);
            double theta = std::asin(n.v);

            double u = 0.5 + (phi / (2 * M_PIf32));
            double v = 0.5 - (theta / M_PIf32);

            double sines = std::sin(e_ * u / (2 * M_PIf32)) * std::sin(e_ * v / (2 * M_PIf32));
            return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, sines > 0 ? C1_ : C2_);
        }

    private:
        float Kd_;
        double e_;
        color C1_;
        color C2_;
        float Ks_;
        float ns_;
        float Kt_;
        float eta_;
    };
}


#endif //RAYTRACING_MATERIALS_HH
