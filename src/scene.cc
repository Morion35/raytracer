//
// Created by simon on 2/25/21.
//

#include "scene.hh"

#include <iostream>
#include <algorithm>
#include <random>

raytracing::Image raytracing::Scene::compute_image(const unsigned short width, const unsigned short height, const unsigned compute_depth, bool aliasing, bool dynamic_dof) {

    const auto pixels = camera_.get_pixels(width, height, aliasing);

    Image image(width, height);
    const auto sources = camera_.source();
    const double e = camera_.get_focal();
    const auto transformer = camera_.transformer();
    unsigned count = 0;
    std::mt19937_64 rd;
    auto unif = std::uniform_real_distribution<double>(-e / 2, e / 2);

#pragma omp parallel for
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            const auto& pixel = pixels[i][j];
            double r = 0, g = 0, b = 0;

            color mean = color();
            unsigned n = pixel.size() * sources.size();
            if (dynamic_dof) {
                p3 src = p3();
                unsigned nc = 0;
                unsigned min = 2;
                color precendent_mean = mean;
                rd.seed(i * width + j);
                do {
                    nc++;
                    precendent_mean = mean;
                    for (const auto& source : sources) {
                        src = source + transformer(vec3(unif(rd), unif(rd), 0));
                        for (const auto& dst : pixel) { // aliasing
                            auto res = compute_ray(src, (dst - src).normalize(), compute_depth).value_or(color());
                            r += res.r;
                            g += res.g;
                            b += res.b;
                        }
                    }
                    unsigned div = n * nc;
                    mean = color(r / div, g / div, b / div);
                } while ((nc < min || mean.diff(precendent_mean) > 1e-5));
            } else {
                for (const auto& source : sources) {
                    for (const auto& dst : pixel) { // antialiasing
                        auto res = compute_ray(source, (dst - source).normalize(), compute_depth).value_or(color());
                        r += res.r;
                        g += res.g;
                        b += res.b;
                    }
                }
                mean = color(r / n, g / n, b / n);
            }
            image.set_pixel(i, j, mean);
        }
#pragma omp critical
        ++count;
        if (count % (height/10) == 0) {
            std::cout << "completion: " << count * 100. / height << "%\n";
        }
    }
    return image;
}

std::optional<raytracing::vec3> raytracing::Scene::refract(const raytracing::vec3 &I, raytracing::vec3 norm, float eta)
{
    double cos_init = I.normalize() * norm.normalize();

    if (cos_init < 0) {
        eta = 1 / eta;
        cos_init = -cos_init;
    }
    else {
        norm = -norm;
    }

    double k = 1 - eta * eta * (1 - cos_init * cos_init);

    if (k < 0)
        return std::nullopt;
    else
        return eta * I + (eta * cos_init - std::sqrt(k)) * norm;
}

double raytracing::Scene::fresnel(const raytracing::vec3 &I, const raytracing::vec3 &norm, float eta)
{
    double cos_init = norm.normalize() * I.normalize();
    double sin_out;
    if (cos_init > 0)
        sin_out = std::sqrt(std::max(0., 1 - cos_init * cos_init)) * eta;
    else {
        sin_out = std::sqrt(std::max(0., 1 - cos_init * cos_init)) / eta;
        cos_init = -cos_init;
    }

    if (sin_out >= 1)
        return 1;

    double cos_out = std::sqrt(std::max(0., 1 - sin_out * sin_out));
    double Rs = (eta * cos_init - cos_out) / (eta * cos_init + cos_out);
    double Rp = (cos_init - eta * cos_out) / (cos_init + eta * cos_out);
    return (Rs * Rs + Rp * Rp) / 2;
}

std::optional<raytracing::color> raytracing::Scene::compute_ray(const raytracing::p3 &s, const raytracing::vec3 &ray, unsigned count) const {

    auto hit = cast_ray(s, ray);
    if (hit) {
        auto [cp, n, Kd, Ks, ns, Kt, eta, c] = hit.value();
        auto O = (cp - s).normalize();

        if (O * n > 0) {
            n = -n;
        }

        auto res = c * ambiant_ * Kd;
        auto S = O - 2 * (O * n) * n;

        for (const auto &light : lights_) {
            auto i = light->intensity(cp, *this);

            auto L = (light->position() - cp).normalize();
            auto LN = std::abs(n * L);

            if (LN >= 0) {
                auto SL = S * L;

                res += c * i * Kd * LN;
                if (SL >= 0)
                    res += i * Ks * std::pow(SL, ns);
            }
        }

        if (count > 0) {
            color reflected;
            color refracted;
            double Kr = 1;
            if (Ks) {
                auto ref = compute_ray(cp, S, count - 1);
                if (ref) {
                    reflected = ref.value();
                }
            }
            if (Kt) {
                auto T = refract(O, n, eta);
                if (T) {
                    auto refract = compute_ray(cp, T.value(), count - 1);
                    if (refract) {
                        refracted = refract.value();
                    }
                    if (Ks) {
                        Kr = fresnel(O, n, eta);
                    } else {
                        Kr = 1 - Kt;
                    }
                }
            }
            res += reflected * Ks * Kr + refracted * (1 - Kr) * ((Ks != 0) ? Ks : 1);
        }
        return res;
    }
    return std::nullopt;
}

std::optional<std::tuple<raytracing::p3, raytracing::vec3, float, float, float, float, float, raytracing::color>> raytracing::Scene::cast_ray(const raytracing::p3 &s, const raytracing::vec3 &ray) const {
    std::optional<p3> closest_point = std::nullopt;
    const Object* closest_object = nullptr;
    double min_dist = std::numeric_limits<double>::infinity(); // never compared before initialization

    for (const auto& object : objects_) {
        const auto hit = object->hit(s, ray);
        if (hit) {
            auto &[intersection_point, intersect_object] = hit.value();
            double sqr = (intersection_point - s).square();
            if (sqr > .00001 && sqr < min_dist) {
                closest_point = intersection_point;
                min_dist = sqr;
                closest_object = intersect_object;
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

std::vector<std::tuple<raytracing::color, double>>
raytracing::Scene::light_passing_objects(const raytracing::p3 &begin, const raytracing::p3 &end) const {
    const vec3 ray = (end - begin);

    std::vector<std::tuple<raytracing::color, double>> passing_values;

    for (const auto& object : objects_) {
        const auto hit = object->hit(begin, ray);
        if (hit) {
            auto &[intersection_point, intersect_object] = hit.value();
            double sqr = (intersection_point - begin).square();
            double sqr_end = (intersection_point - end).square();
            if (sqr > .00001 && sqr_end > .00001 && sqr < ray.square()) {
                auto [Kd, Ks, ns, Kt, eta, c] = intersect_object->texture(intersection_point).value();
                if (Kt) {
                    passing_values.emplace_back(std::tuple(c, Kd));
                } else {
                    return std::vector<std::tuple<raytracing::color, double>>(1, std::tuple(c, 1));
                }
            }
        }
    }
    return passing_values;
}

inline double in(double p1, double p2, double t) {
    if (p1 == p2) {
        return p1;
    }
    return (t - p1) / (p2 - p1);
}