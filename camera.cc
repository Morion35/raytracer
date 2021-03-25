//
// Created by simon on 2/25/21.
//

#include "camera.hh"
#include "objects.hh"
#include "iostream"

std::vector<std::vector<raytracing::vec3>> raytracing::Camera::get_rays(unsigned short width, unsigned short height) const {
    auto n = (P_ - C_);

    auto plane = raytracing::Plane(n, dz_, nullptr);
    auto rx = n.rotateY(ax_);
    auto ry = n.rotateX(ay_);

    p3 px = plane.intersect(C_, rx).value();
    p3 py = plane.intersect(C_, ry).value();

    p3 middle(P_.x, P_.y, dz_.z);

    double plane_width = middle.dist(px) * 2;
    double plane_height = middle.dist(py) * 2;
    vec3 dx = vec3(plane_width / width, 0., 0);
    vec3 dy = vec3(0, plane_height / height, 0);

    vec3 begin = p3(P_.x - plane_width / 2., P_.y + plane_height / 2., dz_.z) + dx - dy - C_;
    std::vector<std::vector<vec3>> rays(height, std::vector<vec3>(width));
    for (unsigned short i = 0; i < height; ++i) {
        for (unsigned short j = 0; j < width; ++j) {
            rays[i][j] = (begin + (dx * j) - (dy * i));
        }
    }
    return rays;
}