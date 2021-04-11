//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_MATERIALS_HH

#define RAYTRACING_MATERIALS_HH

#include "type.hh"
#include "image.hh"

#include <tuple>

namespace raytracing {
    using texture_values = std::tuple<float, float, float, float, float, color>;

    class Texture_Material {
    public:
        virtual texture_values texture(double u, double v) const = 0;
    };

    class Uniform_Material : public Texture_Material {
    public:
        explicit Uniform_Material(color C, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
            : C_(C), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const double u, const double v) const override {
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

    class Paved_Material : public Texture_Material {
    public:
        explicit Paved_Material(color C1, color C2, double e, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
                : C1_(C1), C2_(C2), e_(e), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const double u, const double v) const override {
            unsigned u2 = std::floor(u * e_);
            unsigned v2 = std::floor(v * e_);

            return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, ((u2 + v2) % 2 == 0) ? C1_ : C2_);
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

    class Sin_Paved_Material : public Texture_Material {
    public:
        explicit Sin_Paved_Material(color C1, color C2, double e, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
                : C1_(C1), C2_(C2), e_(e), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const double u, const double v) const override {
            double sines = std::sin(M_PIf32 / e_ * u) * std::sin(M_PIf32 / e_ * v);
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

    class Ring_Material : public Texture_Material {
    public:
        explicit Ring_Material(color C1, color C2, double e, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
                : C1_(C1), C2_(C2), e_(e), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const double u, const double v) const override {
            double u2 = u * e_;
            double v2 = v * e_;
            return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, (int) (std::sqrt(u2 * u2 + v2 * v2)) % 2 == 0 ? C1_ : C2_);
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

    class Ppm_Material : public Texture_Material {
    public:
        explicit Ppm_Material (const std::string& filename, double e, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
                : texture_image_(filename), e_(e), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const double u, const double v) const override {
            double u2 = std::fmod(u / e_, 1.);
            double v2 = std::fmod(v / e_, 1.);

            if (u2 < 0) { u2 += 1.; }
            if (v2 < 0) { v2 += 1.; }

            v2 = 1 - v2;

            auto x = (unsigned) (texture_image_.width() * u2);
            auto y = (unsigned) (texture_image_.height() * v2);

            return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, texture_image_.data()[y][x]);
        }

    private:
        raytracing::Image texture_image_;
        double e_;
        float Kd_;
        float Ks_;
        float ns_;
        float Kt_;
        float eta_;
    };

    class Cube_Material : public Texture_Material {
    public:
        explicit Cube_Material (color main, color ul, color ur, color bl, color br, double e, float Kd = 1, float Ks = 0, float ns = 1, float Kt = 0, float eta = 1)
                : main_(main), ul_(ul), ur_(ur), bl_(bl), br_(br), e_(e), Ks_(Ks), Kd_(Kd), ns_(ns), Kt_(Kt), eta_(eta) {};

        texture_values texture(const double u, const double v) const override {
            if (v > 0.8) {
                if (u < 0.2)
                    return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, ul_);
                if (u > 0.8)
                    return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, ur_);
            }
            else if (v < 0.2) {
                if (u < 0.2)
                    return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, bl_);
                if (u > 0.8)
                    return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, br_);
            }

            return std::tuple(Kd_, Ks_, ns_, Kt_, eta_, main_);
        }

    private:
        color main_;
        color ul_;
        color ur_;
        color br_;
        color bl_;
        double e_;
        float Kd_;
        float Ks_;
        float ns_;
        float Kt_;
        float eta_;
    };
}

#endif //RAYTRACING_MATERIALS_HH