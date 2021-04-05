//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_LIGHT_HH

#define RAYTRACING_LIGHT_HH

#include "type.hh"
#include "scenetype.hh"

namespace raytracing {

    class Light {
    public:
        virtual double intensity(const p3&, const SceneType&) const = 0;
        virtual p3 position() const = 0;
    };

    class Point_Light : public Light {
    public:

        Point_Light(raytracing::p3 p, float I) : p_(p), I_(I) {};

        p3 position() const override { return p_; }
        double intensity(const p3& p, const SceneType&) const override;

    private:
        p3 p_;
        double I_;
    };

    class Square_Light : public Light {
    public:

        Square_Light(raytracing::p3 p, double d, unsigned char step, double I) : center_(p), d_(d), step_(step), I_(I) { };

        p3 position() const override { return center_; }
        double intensity(const p3& p, const SceneType&) const override;

    private:
        p3 center_;
        double d_;
        unsigned char step_;

        double I_;
    };
}


#endif //RAYTRACING_LIGHT_HH
