//
// Created by simon on 2/25/21.
//

#include <map>

#include "objects.hh"
#include "scene.hh"

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
    if (t1 >= 0.00001 && t1 < t2)
        return std::optional(p + t1 * v);
    if (t2 >= 0.00001)
        return std::optional(p + t2 * v);
    return std::nullopt;
}

std::optional<texture_values> Sphere::texture(const p3 &p) const {
    return std::optional(material_->texture(p));
}

std::optional<texture_values> Plane::texture(const p3 &p) const {
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
    const double e = 0.000001;
    const auto a = - ray * n_;
    if (a < e && a > -e) {
        return std::nullopt;
    }
    const auto ia = 1.0 / a;
    const vec3 AO = o - c1_;
    const vec3 DAO = AO.cross(ray);
    const double u = ia * CA_ * DAO;
    if (u < -e || u >= 1. + e) {
        return std::nullopt;
    }
    const double v = - ia * BA_ * DAO;
    if (v < -e || u + v >= (1.0 +  2. * e)) {
        return std::nullopt;
    }
    const double t = ia * AO * n_;
    if (t > e) {
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

std::optional<vec3> Blob::norm(const p3 &p) const {
    return std::nullopt;
}

std::optional<p3> Blob::intersect(const p3 &o, const vec3 &ray) const noexcept {
    if (!box_.intersect(o, ray)) {
        return std::nullopt;
    }
    else {
        std::optional<p3> closest_point;
        std::shared_ptr<SmoothTriangle> closest_object;
        double min_dist = std::numeric_limits<double>::infinity(); // never compared before initialization

        for (const auto& triangle : triangles_) {
            const auto intersection_point = triangle->intersect(o, ray);

            if (intersection_point) {
                double sqr = (intersection_point.value() - o).square();
                if (sqr > .00001 && sqr < min_dist) {
                    closest_point = intersection_point;
                    min_dist = sqr;
                    closest_object = triangle;
                }
            }
        }
        if (closest_point)
            return closest_point.value();
        return std::nullopt;
    }
}

inline double in(double p1, double p2, double t) {
    return (t - p1) / (p2 - p1);
}

void Blob::initialize() {
    if (triangles_.empty()) {
        const p3 origin = c_ - e_ / 2;
        p3 p = origin;
        unsigned n = e_ / d_ + 1;
        const auto d = d_;
        std::vector<std::vector<std::vector<double>>> potentials(n, std::vector<std::vector<double>>(n, std::vector<double>(n)));
        for (auto i = 0; i < n; ++i) {
            p.y = origin.y;
            for (auto j = 0; j < n; ++j) {
                p.z = origin.z;
                for (auto k = 0; k < n; ++k) {
                    potentials[i][j][k] = potential(p);
                    p.z += d_;
                }
                p.y += d_;
            }
            p.x += d_;
        }
        const p3 static_edge_map[12] = {
                p3(d / 2, 0, 0),
                p3(d, d / 2, 0),
                p3(d / 2, d, 0),
                p3(0, d / 2, 0),
                p3(d / 2, 0, d),
                p3(d, d / 2, d),
                p3(d / 2, d, d),
                p3(0, d / 2, d),
                p3(0, 0, d / 2),
                p3(d, 0, d / 2),
                p3(d, d, d / 2),
                p3(0, d, d / 2)
        };

        p = p3(origin);
        for (auto i = 0; i < n - 1; ++i) {
            p.y = origin.y;
            for (auto j = 0; j < n - 1; ++j) {
                p.z = origin.z;
                for (auto k = 0; k < n - 1; ++k) {
                    int index = 0;
                    double po1 = potentials[i][j][k];
                    double po2 = potentials[i + 1][j][k];
                    double po3 = potentials[i + 1][j + 1][k];
                    double po4 = potentials[i][j + 1][k];
                    double po5 = potentials[i][j][k + 1];
                    double po6 = potentials[i + 1][j][k + 1];
                    double po7 = potentials[i + 1][j + 1][k + 1];
                    double po8 = potentials[i][j + 1][k + 1];
                    if (po1 < t_) index |= 1;
                    if (po2 < t_) index |= 2;
                    if (po3 < t_) index |= 4;
                    if (po4 < t_) index |= 8;
                    if (po5 < t_) index |= 16;
                    if (po6 < t_) index |= 32;
                    if (po7 < t_) index |= 64;
                    if (po8 < t_) index |= 128;
                    const auto edges = SQUARE_MAP[index];
                    const p3 edge_map[12] = {
                            p3(in(po1, po2, t_) * d, 0, 0),
                            p3(d, in(po2, po3, t_) * d, 0),
                            p3(in(po4, po3, t_) * d, d, 0),
                            p3(0, in(po1, po4, t_) * d, 0),
                            p3(in(po5, po6, t_) * d, 0, d),
                            p3(d, in(po6, po7, t_) * d, d),
                            p3(d * in(po8, po7, t_), d, d),
                            p3(0, d * in(po5, po8, t_), d),
                            p3(0, 0, d * in(po1, po5, t_)),
                            p3(d, 0, d * in(po2, po6, t_)),
                            p3(d, d, d * in(po3, po7, t_)),
                            p3(0, d, d * in(po4, po8, t_))
                    };
                    for (auto x = 0; x < 5; ++x) {
                        if (edges[3 * x] == -1)
                            break;
                        p3 v1 = p + edge_map[edges[3 * x]];
                        p3 v2 = p + edge_map[edges[3 * x + 1]];
                        p3 v3 = p + edge_map[edges[3 * x + 2]];
                        vec3 n1 = smooth_norm(v1);
                        vec3 n2 = smooth_norm(v2);
                        vec3 n3 = smooth_norm(v3);
                        //vec3 n1 = v1 - center;
                        //vec3 n2 = v2 - center;
                        //vec3 n3 = v3 - center;
                        triangles_.push_back(std::make_shared<SmoothTriangle>(v1, v2, v3, n1, n2, n3, texture_));
                    }
                    p.z += d;
                }
                p.y += d;
            }
            p.x += d;
        }
        std::cout << "Finished Blob initialization\n";
    }
}

vec3 Blob::smooth_norm(const p3 &p) const {
    double potential = 0;
    double maxpotential = -1;
    vec3 norm;
    vec3 maxnorm;
    for (const p3& src : s_) {
        auto n = (p - src);
        auto dist = n.square();
        if (1.0 / dist >= maxpotential) {
            maxpotential = 1.0 / dist;
            maxnorm = n;
        }
        norm += n / dist;
        potential += 1.0 / dist;
    }
    norm /= potential;
    return norm.normalize();
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
    const auto v = (DCW * DBA - DBW * DCBA) * ia;
    const auto norm = n1_ * (1 - u - v) + n2_ * u + n3_ * v;
    return norm.normalize();
}

std::optional<vec3> Cylinder::norm(const p3 &p) const {
    return std::optional<vec3>();
}

std::optional<p3> Cylinder::intersect(const p3 &, const vec3 &) const noexcept {
    return std::optional<p3>();
}

std::optional<p3> Box::intersect(const p3 &o, const vec3 &ray) const noexcept {
    const auto invray = vec3(1. / ray.u, 1. / ray.v, 1. / ray.w);
    const auto min = c_ - e_ / 2;
    const auto max = c_ + e_ / 2;
    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    if (invray.u >= 0.00001) {
        tmin = (min.x - o.x) * invray.u;
        tmax = (max.x - o.x) * invray.u;
    } else {
        tmin = (max.x - o.x) * invray.u;
        tmax = (min.x - o.x) * invray.u;
    }
    if (invray.v >= 0.00001) {
        tymin = (min.y - o.y) * invray.v;
        tymax = (max.y - o.y) * invray.v;
    } else {
        tymin = (max.y - o.y) * invray.v;
        tymax = (min.y - o.y) * invray.v;
    }
    if ((tmin > tymax + 0.0001) || (tmax < tymin - 0.0001))
        return std::nullopt;
    if (tmin < tymin) tmin = tymin;
    if (tmax > tymax) tmax = tymax;
    if (invray.w >= 0.00001) {
        tzmin = (min.z - o.z) * invray.w;
        tzmax = (max.z - o.z) * invray.w;
    } else {
        tzmin = (max.z - o.z) * invray.w;
        tzmax = (min.z - o.z) * invray.w;
    }
    if ((tmin > tzmax + 0.0001) || (tmax < tzmin - 0.0001))
        return std::nullopt;
    if (tmin < tzmin) tmin = tzmin;
    if (tmax > tzmax) tmax = tzmax;
    if (tmin < 0.00001) {
        if (tmax > 0.00001) {
            return o + tmax * ray;
        }
    } else if (tmin > 0.00001) {
        return o + tmin * ray;
    }
    return std::nullopt;
}

std::optional<vec3> Box::norm(const p3 &p) const {
    double epsilon = 0.0001;
    vec3 v = p - c_;
    v = vec3(v.u * v.u, v.v * v.v, v.w * v.w);
    double d2 = e_ * e_ / 4;
    if (v.u < d2 + epsilon && v.u > d2 - epsilon) {
        return vec3(-1, 0, 0);
    }
    if (v.v < d2 + epsilon && v.v > d2 - epsilon) {
        return vec3(0, -1, 0);
    }
    if (v.w < d2 + epsilon && v.w > d2 - epsilon) {
        return -vec3(0, 0, -1);
    }
    return std::nullopt;
}

std::optional<std::tuple<p3, const Object*>> Blob::hit(const p3 &o, const vec3 &ray) const noexcept {
    if (!box_.intersect(o, ray)) {
        return std::nullopt;
    }
    else {
        std::optional<p3> closest_point;
        std::shared_ptr<const Object> closest_object;
        double min_dist = std::numeric_limits<double>::infinity(); // never compared before initialization

        for (const auto& triangle : triangles_) {
            const auto intersection_point = triangle->intersect(o, ray);

            if (intersection_point) {
                double sqr = (intersection_point.value() - o).square();
                if (sqr > .00001 && sqr < min_dist) {
                    closest_point = intersection_point;
                    min_dist = sqr;
                    closest_object = triangle;
                }
            }
        }

        if (closest_point)
            return std::tuple(closest_point.value(), closest_object.get());
        return std::nullopt;
    }
}