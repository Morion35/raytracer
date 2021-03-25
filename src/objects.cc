//
// Created by simon on 2/25/21.
//

#include <map>
#include "objects.hh"
#include "iostream"

using namespace raytracing;

std::optional<p3> Sphere::intersect(const p3& p, const vec3& v) const noexcept {
    vec3 dist = p - center_;
    double a = v.square();
    double b =  2 * v * dist;
    double c = dist * dist - r_ * r_;
    double d = b * b - 4 * a * c;
    if (d < 0) {
        return std::nullopt;
    }
    double t1 = (-b - std::sqrt(d)) / (2 * a);
    double t2 = (-b + std::sqrt(d)) / (2 * a);
    if (t1 >= 0 && t1 < t2)
        return std::optional(p + t1 * v);
    if (t2 >= 0)
        return std::optional(p + t2 * v);
    return std::nullopt;
}

std::optional<std::tuple<float, float, float, color>> Sphere::texture(const p3 &p) const {

    auto sqr = (p - center_).square();
    if (sqr <= (r_ * r_ + 0.0001) && sqr >= (r_ * r_ - 0.0001)) {
        return std::optional(material_->texture(p));
    }

    return std::nullopt;

}

std::optional<std::tuple<float, float, float, color>> Plane::texture(const p3 &p) const {
    return std::optional(material_->texture(p));
}

std::optional<vec3> Sphere::norm(const p3& p) const {

    auto dist = (p - center_).normalize();

    return dist;
}

std::optional<vec3> Plane::norm(const p3 &) const {
    return n_;
}

std::optional<p3> Plane::intersect(const p3 &p, const vec3 &v) const noexcept {
    double a = n_ * v;
    if (a == 0)
        return std::nullopt;

    double t = (d_ - n_ * p.vec()) / a;

    if (t <= 0)
        return std::nullopt;

    return p + t * v;
}

std::ostream& operator<<(std::ostream& o, const raytracing::Sphere&) {
    o << "Sphere";
    return o;
}

std::ostream& operator<<(std::ostream& o, const raytracing::Plane&) {
    o << "Plane";
    return o;
}

std::optional<p3> Triangle::intersect(const p3 &o, const vec3 &ray) const noexcept {
    const double e = 0.0001;
    const auto a = - ray * n_;
    if (a < e && a > -e) {
        return std::nullopt;
    }
    const auto ia = 1.0 / a;
    const vec3 AO = o - c1_;
    const vec3 DAO = AO.cross(ray);
    const auto u = ia * CA_ * DAO;
    if (u < -e || u > 1 + e) {
        return std::nullopt;
    }
    const auto v = - ia * BA_ * DAO;
    if (v < -e || v + u > 1.0 + e) {
        return std::nullopt;
    }
    const auto t = ia * AO * n_;
    if (t > 0) {
        return o + ray * t;
    }
    return std::nullopt;
}

double Blob::potential(const p3 &p) const {
    double potential = 0;
    for (const p3& src : s_) {
        auto dist = (src - p).square();
        potential += 1.0 / dist;
    }
    return potential;
}

std::optional<vec3> SmoothTriangle::norm(const p3 &p) const {
    const double e = 0.0001;
    const double DBA = BA_.square();
    const double DCA = CA_.square();
    const double DCBA = CA_ * BA_;
    const double DCW = CA_ * (p - c1_);
    const double DBW = BA_ * (p - c1_);

    const double ia = 1.0 / (DCA * DBA - DCBA * DCBA);
    const double u = (DBW * DCA - DCW * DCBA) * ia;
    if (u < -e || u > 1 + e) {
        return std::nullopt;
    }
    const auto v = (DCW * DBA - DBW * DCBA) * ia;
    if (v < -e || v + u > 1.0 + e) {
        return std::nullopt;
    }
    const auto norm = n1_ * (1 - u - v) + n2_ * u + n3_ * v;
    return norm.normalize();
}

std::optional<vec3> Cylinder::norm(const p3 &p) const {
    auto diag = p - c1_;


}
