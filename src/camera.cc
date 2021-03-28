//
// Created by simon on 2/25/21.
//

#include "camera.hh"
#include "objects.hh"
#include "iostream"

std::vector<std::vector<std::vector<raytracing::vec3>>> raytracing::Camera::get_rays(unsigned short width, unsigned short height, bool aliasing) const {
    auto n = (P_ - C_).normalize();
    auto right = sky_.cross(n).normalize();
    std::cout << right;
    auto up = n.cross(right).normalize();
    std::cout << up;
    auto x = vec3(right.u, up.u, n.u);
    auto y = vec3(right.v, up.v, n.v);
    auto z = vec3(right.w, up.w, n.w);

    double plane_width = std::tan(ax_) * 2 * dz_;
    double plane_height = std::tan(ay_) * 2 * dz_;
    vec3 dx = vec3(plane_width / width, 0., 0);
    vec3 dy = vec3(0, plane_height / height, 0);
    dx = vec3(x.dot(dx), y.dot(dx), z.dot(dx));
    dy = vec3(x.dot(dy), y.dot(dy), z.dot(dy));

    vec3 begin = vec3(P_.x - plane_width / 2., P_.y + plane_height / 2., dz_);
    begin = vec3(x.dot(begin), y.dot(begin), z.dot(begin));
    begin = begin + dx - dy - C_;
    std::vector<std::vector<std::vector<vec3>>> rays(height, std::vector<std::vector<vec3>>(width));
    for (unsigned short i = 0; i < height; ++i) {
        for (unsigned short j = 0; j < width; ++j) {
            auto center_ray = (begin + (dx * j) - (dy * i));
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