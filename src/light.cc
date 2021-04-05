//
// Created by simon on 2/25/21.
//

#include "light.hh"

double raytracing::Point_Light::intensity(const raytracing::p3 &p, const raytracing::SceneType &scene) const {
    auto ray = p - p_;
    auto hidden_from_light = scene.cast_ray(p_, (ray));

    if (hidden_from_light) {
        p3 cpu = std::get<0>(hidden_from_light.value());
        if ((cpu - p).square() >= .00001 && (cpu - p).square() <= (ray).square())
            return 0;
    }

    return I_ / p.dist(p_);
}

double raytracing::Square_Light::intensity(const raytracing::p3 &p, const raytracing::SceneType &scene) const {
    vec3 sky(0, 1, 0);

    auto ray = center_ - p;
    double ray_dist = p.dist(center_);

    auto n = ray.normalize();
    auto right = sky.cross(n).normalize();
    auto up = n.cross(right).normalize();
    auto x = vec3(right.u, up.u, n.u);
    auto y = vec3(right.v, up.v, n.v);
    auto z = vec3(right.w, up.w, n.w);

    vec3 dx = vec3(d_ / step_, 0., 0);
    vec3 dy = vec3(0, d_ / step_, 0);
    dx = vec3(x.dot(dx), y.dot(dx), z.dot(dx));
    dy = vec3(x.dot(dy), y.dot(dy), z.dot(dy));

    vec3 begin = vec3(-d_ / 2., d_ / 2., 0);
    begin = vec3(x.dot(begin), y.dot(begin), z.dot(begin));
    begin = begin + dx / 2 - dy / 2 + center_;

    double count = 0;
    for (unsigned short i = 0; i < step_; ++i) {
        for (unsigned short j = 0; j < step_; ++j) {
            auto center_ray = (begin + (dx * j) - (dy * i)).p();
            auto hidden_from_light = scene.cast_ray(center_ray, p - center_ray);

            if (hidden_from_light) {
                p3 hit = std::get<0>(hidden_from_light.value());
                if ((hit - p).square() >= .00001 && (hit - p).square() <= (p - center_ray).square()) {
                    continue;
                }
            }

            count++;
        }
    }

    return (count / (step_ * step_)) * I_ / ray_dist;
}