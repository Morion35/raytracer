//
// Created by simon on 2/25/21.
//

#ifndef RAYTRACING_LIGHT_HH

#include "type.hh"

namespace raytracing {
    using namespace raytracing;

    class Light {
    public:
        virtual double intensity(const p3&) const = 0;
        virtual p3 position() const = 0;
    };

    class Point_Light : public Light {
    public:

        Point_Light(raytracing::p3 p, float I) : p_(p), I_(I) {};

        p3 position() const override { return p_; }
        double intensity(const p3&) const override { return I_; }

    private:
        p3 p_;
        double I_;
    };
}

#define RAYTRACING_LIGHT_HH

#endif //RAYTRACING_LIGHT_HH
