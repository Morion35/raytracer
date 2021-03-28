//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_OBJECTS_HH

#include "type.hh"
#include "materials.hh"
#include <optional>
#include <string>
#include <memory>
#include <utility>
#include <functional>
#include <iostream>

namespace raytracing {
    class Object {
    public:
        virtual std::optional<p3> intersect(const p3&, const vec3&) const noexcept = 0;
        virtual std::optional<vec3> norm(const p3&) const = 0;
        virtual std::optional<std::tuple<float, float, float, color>> texture(const p3&) const = 0;
        virtual std::string name() const = 0;
    };

    class Sphere : public Object {
    public:
        template<class T = Texture_Material>
        Sphere(const p3& center, double r, T material) : center_(center), r_(r) {
            material_ = new T(material);
        };

        std::optional<p3> intersect(const p3&, const vec3&) const noexcept override;
        std::optional<vec3> norm(const p3&) const override;
        std::optional<std::tuple<float, float, float, color>> texture(const p3&) const override;
        std::string name() const override { return "Sphere"; };

    private:
        p3 center_;
        double r_;
        Texture_Material* material_;
    };

    class Plane : public Object {

    public:
        Plane(const vec3& n, const p3& p, std::shared_ptr<Texture_Material>  material) : n_(n), p_(p), material_(std::move(material)) {
            d_ = n * p.vec();
            n_ = n_.normalize();
        };

        std::optional<p3> intersect(const p3&, const vec3&) const noexcept override;
        std::optional<vec3> norm(const p3&) const override;
        std::optional<std::tuple<float, float, float, color>> texture(const p3&) const override;
        std::string name() const override { return "Plane"; };

    private:
        vec3 n_;
        p3 p_;
        double d_;
        std::shared_ptr<Texture_Material> material_;
    };

    class Triangle : public Object {
    public:
        Triangle(const p3& c1, const p3& c2, const p3& c3, std::shared_ptr<Texture_Material> texture) :
                c1_(c1), c2_(c2), c3_(c3), texture_(std::move(texture)) {
            BA_ = (c2 - c1);
            CA_ = (c3 - c1);
            n_ = BA_.cross(CA_);
            nn_ = n_.normalize();
        };

        std::optional<vec3> norm(const p3&) const override { return nn_; }

        std::optional<p3> intersect(const p3&, const vec3&) const noexcept override;

        std::optional<std::tuple<float, float, float, color>> texture(const p3& p) const override {
            return texture_->texture(p);
        }

        std::string name() const override {
            std::cout << c1_.vec() << "\n";
            std::cout << c2_.vec() << "\n";
            std::cout << c3_.vec() << "\n";
            return "Triangle: ";
        };
    protected:

        vec3 n_;
        vec3 nn_;
        vec3 BA_;
        vec3 CA_;
        p3 c1_;
        p3 c2_;
        p3 c3_;

        std::shared_ptr<Texture_Material> texture_;
    };

    class SmoothTriangle : public Triangle {
    public:
        SmoothTriangle(const p3& c1, const p3& c2, const p3& c3, const vec3& n1, const vec3& n2, const vec3& n3, std::shared_ptr<Texture_Material> texture) :
                Triangle(c1, c2, c3, std::move(texture)), n1_(n1.normalize()), n2_(n2.normalize()), n3_(n3.normalize()) {};

        std::optional<vec3> norm(const p3&) const override;

    private:
        vec3 n1_;
        vec3 n2_;
        vec3 n3_;

    };

    class Cube : public Object {
        Cube(const p3& center, double edge) {};
    };

    class Cylinder : public Object {
    public:
        Cylinder(double r, const p3& c1, const p3& c2) : r_(r), c1_(c1), c2_(c2) {};

        std::optional<vec3> norm(const p3&) const override;

        std::optional<p3> intersect(const p3&, const vec3&) const noexcept override;
    private:
        double r_;
        p3 c1_;
        p3 c2_;
    };



    class Blob {
    public:
        Blob(double e, double d, double threshold, const p3& center, std::vector<p3> sources, std::shared_ptr<Texture_Material>&& texture) :
                                                                d_(d), e_(e), t_(threshold),
                                                                s_(std::move(sources)),
                                                                p_(center),
                                                                texture_(texture){};

        double potential(const p3& p) const;

        double d() const { return d_; };

        double e() const { return e_; };

        double threshold() const { return t_; };

        std::shared_ptr<Texture_Material> texture() const { return texture_; };

        p3 center() const { return p_; }

    private:
        std::shared_ptr<Texture_Material> texture_;
        std::vector<p3> s_;
        p3 p_;
        double e_;
        double d_;
        double t_;
    };

}

#define RAYTRACING_OBJECTS_HH

#endif //RAYTRACING_OBJECTS_HH
