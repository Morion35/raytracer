//
// Created by simon on 2/25/21.
//

#include "scene.hh"

#include <iostream>
#include <algorithm>

raytracing::Image raytracing::Scene::compute_image(const unsigned short width, const unsigned short height, const unsigned compute_depth, bool aliasing) {

    const auto pixels = camera_.get_pixels(width, height, aliasing);

    Image image(width, height);
    const auto sources = camera_.source();
    unsigned count = 0;

#pragma omp parallel for
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            const auto& pixel = pixels[i][j];
            unsigned r = 0, g = 0, b = 0;
            unsigned n = pixel.size() * sources.size();
            for (const auto& dst : pixel) {
                for (const auto& src : sources) {
                    auto res = compute_ray(src, dst - src, compute_depth).value_or(color());
                    r += res.r;
                    g += res.g;
                    b += res.b;
                }
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

std::optional<raytracing::vec3> refract(const raytracing::vec3 &I, const raytracing::vec3 &N, const float &Kt)
{
    double cosi = std::clamp(-1., 1., I * N);
    double etai = 1, etat = Kt;
    raytracing::vec3 n = N;

    if (cosi < 0) {
        cosi = -cosi;
    }
    else {
        std::swap(etai, etat);
        n = -N;
    }

    double eta = etai / etat;
    double k = 1 - eta * eta * (1 - cosi * cosi);

    if (k < 0)
        return std::nullopt;
    else
        return eta * I + (eta * cosi - std::sqrt(k)) * n;
}

std::optional<raytracing::color> raytracing::Scene::compute_ray(const raytracing::p3 &s, const raytracing::vec3 &ray, unsigned count) const {

    auto hit = cast_ray(s, ray);
    if (hit) {
        auto [cp, n, Kd, Ks, ns, Kt, eta, c] = hit.value();
        auto O = (cp - s).normalize();

        auto res = c * 0.05;
        auto S = O - 2 * (O * n) * n;

        for (const auto &light : lights_) {
            auto i = light->intensity(cp, *this);

            if (i == 0)
                continue;

            auto L = (light->position() - cp).normalize();
            auto LN = std::abs(n * L);

            if (LN >= 0) {
                auto SL = S * L;

                res += c * i * Kd * LN;
                if (SL >= 0)
                    res += color(255, 255, 255) * i * Ks * std::pow(SL, ns);
            }
        }

        if (count > 0) {
            if (eta) {
                auto T = refract(O, n, eta) ;
                if (T) {
                    auto refract = compute_ray(cp, T, count - 1);
                    if (refract) {
                        auto refr = refract.value();
                        res += refr * Kt;
                    }
                }
            }
            auto reflect = compute_ray(cp, S, count - 1);
            if (reflect) {
                auto ref = reflect.value();
                res += ref * Ks;
            }
        }
        return res;
    }
    return std::nullopt;
}

std::optional<std::tuple<raytracing::p3, raytracing::vec3, float, float, float, float, float, raytracing::color>> raytracing::Scene::cast_ray(const raytracing::p3 &s, const raytracing::vec3 &ray) const {
    std::optional<p3> closest_point;
    std::shared_ptr<const Object> closest_object;
    double min_dist = std::numeric_limits<double>::infinity(); // never compared before initialization

    for (const auto& object : objects_) {
        const auto hit = object->hit(s, ray);
        if (hit) {
            const auto& [intersection_point, object] = hit.value();
            double sqr = (intersection_point - s).square();
            if (sqr > .00001 && sqr < min_dist) {
                closest_point = intersection_point;
                min_dist = sqr;
                closest_object = object;
            }
        }
    }

    if (closest_point) {
        auto norm = closest_object->norm(closest_point.value());

        if (!norm) {
            std::cerr << "Intersection with no norm.";
            return std::nullopt;
        }

        auto texture = closest_object->texture(closest_point.value());

        if (!texture) {
            std::cerr << "Intersection with no texture.";
            return std::nullopt;
        }

        auto [Kd, Ks, ns, Kt, eta, c] = texture.value();
        return std::tuple(closest_point.value(), norm.value(), Kd, Ks, ns, Kt, eta, c);
    }

    return std::nullopt;
}

inline double in(double p1, double p2, double t) {
    if (p1 == p2) {
        return p1;
    }
    return (t - p1) / (p2 - p1);
}