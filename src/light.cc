//
// Created by simon on 2/25/21.
//

#include "light.hh"

raytracing::color raytracing::Point_Light::intensity(const raytracing::p3 &p, const raytracing::SceneType &scene) const {
    auto hidden_from_light = scene.light_passing_objects(p_, p);

    auto intensity = c_;
    if (!hidden_from_light.empty()) {
        for (auto [c, Ks] : hidden_from_light) {
            if (!Ks) {
                return color();
            }
            intensity *= c * Ks;
        }
    }
    return (intensity / p.dist(p_)) * I_;
}

raytracing::color raytracing::Square_Light::intensity(const raytracing::p3 &p, const raytracing::SceneType &scene) const {
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

    double r = 0, g = 0, b = 0;
    for (unsigned short i = 0; i < step_; ++i) {
        for (unsigned short j = 0; j < step_; ++j) {
            auto center_ray = (begin + (dx * j) - (dy * i)).p();
            auto hidden_from_light = scene.light_passing_objects(center_ray, p);
            auto intensity = c_;

            if (!hidden_from_light.empty()) {
                if (std::get<1>(hidden_from_light.front()) == 1) {
                    continue;
                }
                for (auto [c, Kd] : hidden_from_light) {
                    intensity -= color(1., 1., 1.) * Kd + (color(1., 1., 1.) - c) * Kd;
                }
            }
            intensity = intensity * ((center_ray.dist(p) / -I_) + 1);
            r += intensity.r;
            g += intensity.g;
            b += intensity.b;
        }
    }

    unsigned size = step_ * step_;
    return color(r / size, g / size, b / size);
}