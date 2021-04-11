//
// Created by merlin on 4/11/21.
//

#ifndef RAYTRACING_TEXTURES_HH

#define RAYTRACING_TEXTURES_HH

#include "materials.hh"

#include <memory>

namespace raytracing {
    class Texture {
    public:
        virtual material_values texture(const p3& p) const = 0;
    };

    class Uniform_Texture : public Texture {
    public:
        explicit Uniform_Texture(std::shared_ptr<Material> material)
                : material_(std::move(material)) {}

        material_values texture(const p3& p) const override {
            return material_->material(0, 0);
        };

    private:
        std::shared_ptr<Material> material_;
    };

    class Plane_Texture : public Texture {
    public:
        explicit Plane_Texture(std::shared_ptr<Material> material, const double& e)
        : material_(std::move(material)), e_(e) {}

        material_values texture(const p3& p) const override {
            return material_->material(p.x / e_, p.z / e_);
        };

    private:
        std::shared_ptr<Material> material_;
        double e_;
    };

    class Sphere_Texture : public Texture {
    public:
        explicit Sphere_Texture(std::shared_ptr<Material> material, const double& e)
        : material_(std::move(material)), e_(e) {}

        material_values texture(const p3& p) const override {
            p3 p_norm = p.vec().normalize().p();
            double r = p.vec().norm();

            double phi = std::atan2(p_norm.z, p_norm.x);
            double theta = std::asin(p_norm.y);

            double tmp = std::atan(e_ / 2 / r);

            double u = 0.5 + (phi / (2 * M_PIf32));
            double v = 0.5 + (theta / M_PIf32);

            return material_->material(u / tmp, v / tmp);
        };

    private:
        std::shared_ptr<Material> material_;
        double e_;
    };

    class Box_Texture : public Texture {
    public:
        explicit Box_Texture(std::shared_ptr<Material> material, const double& e)
                : material_(std::move(material)), e_(e) {}

        material_values texture(const p3& p) const override {
            double absX = std::fabs(p.x);
            double absY = std::fabs(p.y);
            double absZ = std::fabs(p.z);

            double maxAxis, uc, vc;

            if (p.x >= 0 && absX >= absY && absX >= absZ) { maxAxis = absX; uc = -p.z; vc = p.y; }
            if (p.x < 0 && absX >= absY && absX >= absZ) { maxAxis = absX; uc = p.z; vc = p.y; }

            if (p.y >= 0 && absY >= absX && absY >= absZ) { maxAxis = absY; uc = p.x; vc = -p.z; }
            if (p.y < 0 && absY >= absX && absY >= absZ) { maxAxis = absY; uc = p.x; vc = p.z; }

            if (p.z >= 0 && absZ >= absX && absZ >= absY) { maxAxis = absZ; uc = p.x; vc = p.y; }
            if (p.z < 0 && absZ >= absX && absZ >= absY) { maxAxis = absZ; uc = -p.x; vc = p.y; }

            double u = 0.5 * (uc / maxAxis + 1.);
            double v = 0.5 * (vc / maxAxis + 1.);

            return material_->material(u / e_ * maxAxis * 2, v / e_ * maxAxis * 2);
        };

    private:
        std::shared_ptr<Material> material_;
        double e_;
    };

    class Skybox_Texture : public Texture {
    public:
        explicit Skybox_Texture(std::shared_ptr<Material> material_l, std::shared_ptr<Material> material_r,
                                std::shared_ptr<Material> material_f, std::shared_ptr<Material> material_b,
                                std::shared_ptr<Material> material_u, std::shared_ptr<Material> material_d,
                                const double& e)
                : material_l_(std::move(material_l)), material_r_(std::move(material_r)),
                material_f_(std::move(material_f)), material_b_(std::move(material_b)),
                material_u_(std::move(material_u)), material_d_(std::move(material_d)),
                e_(e) {}

        material_values texture(const p3& p) const override {
            double absX = std::fabs(p.x);
            double absY = std::fabs(p.y);
            double absZ = std::fabs(p.z);

            double u, v;

            if (p.x >= 0 && absX >= absY && absX >= absZ) {
                u = 0.5 * (-p.z / absX + 1.);
                v = 0.5 * (p.y / absX + 1.);

                return material_r_->material(u / e_ * absX * 2, v / e_ * absX * 2);
            }
            if (p.x < 0 && absX >= absY && absX >= absZ) {
                u = 0.5 * (p.z / absX + 1.);
                v = 0.5 * (p.y / absX + 1.);

                return material_l_->material(u / e_ * absX * 2, v / e_ * absX * 2);
            }

            if (p.y >= 0 && absY >= absX && absY >= absZ) {
                u = 0.5 * (p.x / absY + 1.);
                v = 0.5 * (-p.z / absY + 1.);

                return material_u_->material(u / e_ * absY * 2, v / e_ * absY * 2);
            }
            if (p.y < 0 && absY >= absX && absY >= absZ) {
                u = 0.5 * (p.x / absY + 1.);
                v = 0.5 * (p.z / absY + 1.);

                return material_d_->material(u / e_ * absY * 2, v / e_ * absY * 2);
            }

            if (p.z >= 0 && absZ >= absX && absZ >= absY) {
                u = 0.5 * (p.x / absZ + 1.);
                v = 0.5 * (p.y / absZ + 1.);

                return material_f_->material(u / e_ * absZ * 2, v / e_ * absZ * 2);
            }

            u = 0.5 * (-p.x / absZ + 1.);
            v = 0.5 * (p.y / absZ + 1.);

            return material_b_->material(u / e_ * absZ * 2, v / e_ * absZ * 2);
        };

    private:
        std::shared_ptr<Material> material_l_;
        std::shared_ptr<Material> material_r_;
        std::shared_ptr<Material> material_f_;
        std::shared_ptr<Material> material_b_;
        std::shared_ptr<Material> material_u_;
        std::shared_ptr<Material> material_d_;
        double e_;
    };
}

#endif //RAYTRACING_TEXTURES_HH