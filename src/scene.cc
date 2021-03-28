//
// Created by simon on 2/25/21.
//

#include "scene.hh"
#include "iostream"
#include <omp.h>

raytracing::Image raytracing::Scene::compute_image(const unsigned short width, const unsigned short height, const unsigned compute_depth = 0, bool smooth, bool aliasing) {

    compute_blobs(smooth);

    const auto rays = camera_.get_rays(width, height, aliasing);

    Image image(width, height);
    const p3 s = camera_.source();
    unsigned count = 0;

#pragma omp parallel for
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            const auto& pixel_rays = rays[i][j];
            unsigned r = 0, g = 0, b = 0;
            unsigned n = pixel_rays.size();
            for (const auto& ray : pixel_rays) {
                auto res = compute_ray(s, ray, compute_depth).value_or(color());
                r += res.r;
                g += res.g;
                b += res.b;
            }
            image.set_pixel(i, j, color(r / n, g / n, b / n));
        }
#pragma atomic
        ++count;
        if (count % (height/10) == 0) {
#pragma critical
            std::cout << "completion: " << count * 100. / height << "%\n";
        }
    }
    return image;
}

std::optional<raytracing::color> raytracing::Scene::compute_ray(const raytracing::p3 &s, const raytracing::vec3 &ray, unsigned count) const {

    auto hit = cast_ray(s, ray);
    if (hit) {
        auto [cp, co] = hit.value();
        auto O = (cp - s).normalize();
        auto texture = co->texture(cp);
        if (texture) {
            auto[Kd, Ks, ns, c] = texture.value();
            auto no = co->norm(cp);
            auto n = no.value();
            if (n * O < 0) {
                n = -n;
            }
            auto res = c * 0.05;
            auto S = O - 2 * (O * n) * n;
            auto hidden = true;

            for (const auto &light : lights_) {
                auto hidden_from_light = cast_ray(light->position(), (cp - light->position()));
                if (hidden_from_light) {
                    auto[cpu, cou] = hidden_from_light.value();
                    if ((cpu - cp).square() > 0.0001 && (cpu - cp).square() <= (cp - light->position()).square())
                        continue;
                    else
                        hidden = false;
                } else
                    hidden = false;
                auto L = (light->position() - cp).normalize();
                auto LN = std::abs(n * L);

                if (LN >= 0) {
                    auto SL = S * L;
                    res += c * light->intensity(cp) * Kd * LN;
                    if (SL >= 0)
                        res += color(255, 255, 255) * light->intensity(cp) * Ks * std::pow(SL, ns);
                }
            }

            if (count > 0 && !hidden) {
                auto reflect = compute_ray(cp, S, count - 1);
                if (reflect) {
                    auto ref = reflect.value();
                    res += ref * Ks;
                }
            }
            return res;
        }
    }
    return std::nullopt;
}

std::optional<std::tuple<raytracing::p3, std::shared_ptr<raytracing::Object>>> raytracing::Scene::cast_ray(const raytracing::p3 &s, const raytracing::vec3 &ray) const {
    std::optional<p3> closest_point;
    std::optional<std::shared_ptr<Object>> closest_object;
    double min_dist = std::numeric_limits<double>::infinity(); // never compared before initialization

    for (const auto& object : objects_) {
        const auto intersection_point = object->intersect(s, ray);

        if (intersection_point) {
            double sqr = (intersection_point.value() - s).square();
            if (sqr > .0001 && sqr < min_dist) {
                closest_point = intersection_point;
                min_dist = sqr;
                closest_object = object;
            }
        }
    }
    if (closest_point)
        return std::tuple(closest_point.value(), closest_object.value());
    return std::nullopt;
}

inline double in(double p1, double p2, double t) {
    return 1 - (t - p1) / (p2 - p1);
}

void raytracing::Scene::compute_blobs(bool smooth) {
    for (const auto& blob : blobs_) {
        double e = blob.e();
        double d = blob.d();
        double t = blob.threshold();
        p3 center = blob.center();
        const p3 origin = center - e / 2;
        p3 p = origin;
        unsigned n = e / d + 1;
        auto texture = blob.texture();
        std::vector<std::vector<std::vector<double>>> potentials(n, std::vector<std::vector<double>>(n, std::vector<double>(n)));
        for (auto i = 0; i < n; ++i) {
            p.y = origin.y;
            for (auto j = 0; j < n; ++j) {
                p.z = origin.z;
                for (auto k = 0; k < n; ++k) {
                    potentials[i][j][k] = blob.potential(p);
                    p.z += d;
                }
                p.y += d;
            }
            std::cout << "Row: " << i << " finished for potentials\n";
            p.x += d;
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
                    if (po1 < t) index |= 1;
                    if (po2 < t) index |= 2;
                    if (po3 < t) index |= 4;
                    if (po4 < t) index |= 8;
                    if (po5 < t) index |= 16;
                    if (po6 < t) index |= 32;
                    if (po7 < t) index |= 64;
                    if (po8 < t) index |= 128;
                    const auto edges = SQUARE_MAP[index];
                    const p3 edge_map[12] = {
                            p3(in(po1, po2, t) * d, 0, 0),
                            p3(d, in(po2, po3, t) * d, 0),
                            p3(in(po3, po4, t) * d, d, 0),
                            p3(0, in(po4, po1, t) * d, 0),
                            p3(in(po6, po5, t) * d, 0, d),
                            p3(d, in(po7, po6, t) * d, d),
                            p3(d * in(po8, po7, t), d, d),
                            p3(0, d * in(po5, po8, t), d),
                            p3(0, 0, d * in(po5, po1, t)),
                            p3(d, 0, d * in(po6, po2, t)),
                            p3(d, d, d * in(po7, po3, t)),
                            p3(0, d, d * in(po8, po4, t))
                    };
                    for (auto x = 0; x < 5; ++x) {
                        if (edges[3 * x] == -1)
                            break;
                        p3 v1 = p + edge_map[edges[3 * x]];
                        p3 v2 = p + edge_map[edges[3 * x + 1]];
                        p3 v3 = p + edge_map[edges[3 * x + 2]];
                        if (smooth) {
                            vec3 n1 = v1 - center;
                            vec3 n2 = v2 - center;
                            vec3 n3 = v3 - center;
                            objects_.push_back(std::make_shared<SmoothTriangle>(v1, v2, v3, n1, n2, n3, texture));
                        } else
                            objects_.push_back(std::make_shared<Triangle>(v1, v2, v3, texture));
                    }
                    p.z += d;
                }
                p.y += d;
            }
            std::cout << "Row: " << i << " finished for cube marching\n";
            p.x += d;
        }
    }
    std::cout << "Number of objects after cube marching: " << objects_.size() << "\n";
    blobs_.clear();
}

