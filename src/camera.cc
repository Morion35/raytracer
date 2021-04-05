//
// Created by simon on 2/25/21.
//

#include "camera.hh"
#include "objects.hh"
#include "iostream"

raytracing::Camera::Camera(raytracing::p3 origin, p3 viewpoint, vec3 sky, double ax, double ay, double dz, double e, double d) :
                                                                                                        C_(origin),
                                                                                                        P_(viewpoint),
                                                                                                        sky_(sky),
                                                                                                        ax_(ax),
                                                                                                        ay_(ay),
                                                                                                        dz_(dz),
                                                                                                        d_(d),
                                                                                                        e_(e) {
    n_ = (P_ - C_).normalize();
    right_ = sky_.cross(n_).normalize();
    up_ = n_.cross(right_).normalize();
    x_ = vec3(right_.u, up_.u, n_.u);
    y_ = vec3(right_.v, up_.v, n_.v);
    z_ = vec3(right_.w, up_.w, n_.w);
    sources_.push_back(C_);
    if (e_ == 0 || d_ == 0)
        return;
    vec3 dx = vec3(d_, 0., 0);
    vec3 dy = vec3(0, d_, 0);
    dx = transform(dx);
    dy = transform(dy);
    vec3 begin = vec3(-e_ / 2., e_ / 2., 0);
    begin = transform(begin);
    begin = begin + dx / 2 - dy / 2 + C_;
    for (unsigned i = 0; i < e_ / d_; ++i) {
        for (unsigned j = 0; j < e_ / d_; ++j) {
            auto center_point = (begin + (dx * j) - (dy * i)).p();
            sources_.push_back(center_point);
        }
    }
};

std::vector<std::vector<std::vector<raytracing::p3>>> raytracing::Camera::get_pixels(unsigned short width, unsigned short height, bool aliasing) const {
    double plane_width = std::tan(ax_) * 2 * dz_;
    double plane_height = std::tan(ay_) * 2 * dz_;
    vec3 dx = vec3(plane_width / width, 0., 0);
    vec3 dy = vec3(0, plane_height / height, 0);
    dx = transform(dx);
    dy = transform(dy);

    vec3 begin = vec3(-plane_width / 2., plane_height / 2., dz_);
    begin = transform(begin);
    begin = begin + dx / 2 - dy / 2 + C_;

    std::vector<std::vector<std::vector<p3>>> rays(height, std::vector<std::vector<p3>>(width));
    for (unsigned short i = 0; i < height; ++i) {
        for (unsigned short j = 0; j < width; ++j) {
            auto center_ray = (begin + (dx * j) - (dy * i)).p();
            rays[i][j].push_back(center_ray);
            if (aliasing) {
                rays[i][j].push_back(center_ray + dx / 2);
                rays[i][j].push_back(center_ray - dx / 2);
                rays[i][j].push_back(center_ray + dy / 2);
                rays[i][j].push_back(center_ray - dy / 2);
            }
        }
    }
    return rays;
}