//
// Created by simon on 2/23/21.
//

#ifndef RAYTRACING_TYPES_H

#define RAYTRACING_TYPES_H

#include <ostream>
#include <cmath>
#include <optional>

namespace raytracing {

    enum axis {
        X,
        Y,
        Z,
    };

    struct color {
        color() = default;

        color(unsigned red, unsigned green, unsigned blue) : r(red / 255.), g(green / 255.), b(blue / 255.) {};
        color(int red, int green, int blue) : r(red / 255.), g(green / 255.), b(blue / 255.) {};
        color(unsigned char red, unsigned char green, unsigned char blue) : r(red / 255.), g(green / 255.), b(blue / 255.) {};
        color(double red, double green, double blue) : r(red), g(green), b(blue) {};
        color(float red, float green, float blue) : r(red), g(green), b(blue) {};

        double r = 0;
        double g = 0;
        double b = 0;

        bool operator==(const color& c) const {return r == c.r && g == c.g && b == c.b; };
        bool operator!=(const color& c) const {return !(*this == c); };
        color operator*=(double l) {
            r = std::clamp(r * l, 0., 1.);
            g = std::clamp(g * l, 0., 1.);
            b = std::clamp(b * l, 0., 1.);
            return *this;
        }

        color operator/=(double l) {
            r = std::clamp(r / l, 0., 1.);
            g = std::clamp(g / l, 0., 1.);
            b = std::clamp(b / l, 0., 1.);
            return *this;
        }

        color operator/(double l) const { return color(*this) /= l; }

        color operator+=(const color& c) {
            r = std::clamp(r + c.r, 0., 1.);
            g = std::clamp(g + c.g, 0., 1.);
            b = std::clamp(b + c.b, 0., 1.);
            return *this;
        }

        color operator-=(const color& c) {
            r = std::clamp(r - c.r, 0., 1.);
            g = std::clamp(g - c.g, 0., 1.);
            b = std::clamp(b - c.b, 0., 1.);
            return *this;
        }

        color operator-(const color& c) const { return color(*this) -= c; }

        double diff(const color& c) const {
            double rdiff = r - c.r;
            double gdiff = g - c.g;
            double bdiff = b - c.b;
            return std::sqrt(rdiff * rdiff + bdiff * bdiff + gdiff * gdiff);
        }

        color operator+(const color& c) const { return color(*this) += c; }

        color operator*(double l) const { return color(*this) *= l; };


        color operator*=(const color& c) {
            r *= c.r;
            g *= c.g;
            b *= c.b;
            return *this;
        }

        color operator*(const color& c) const {
            return color(*this) *= c;
        }
    };

    struct p3;

    struct vec3 {
        double u = 0;
        double v = 0;
        double w = 0;

        constexpr vec3(double u, double v, double w) : u(u), v(v), w(w) {};

        vec3() = default;

        constexpr vec3 operator*=(double l) {
            u *= l;
            v *= l;
            w *= l;
            return *this;
        }

        constexpr vec3 operator/=(double l) {
            u /= l;
            v /= l;
            w /= l;
            return *this;
        }

        constexpr vec3 operator*(double l) const { return vec3(*this) *= l; }
        constexpr vec3 operator/(double l) const { return vec3(*this) /= l; }

        constexpr double dot(const vec3& vec) const { return u * vec.u + v * vec.v + w * vec.w; }

        constexpr double operator*(const vec3& vec) const { return dot(vec); }


        constexpr vec3 operator-=(const vec3 &vec) {
            u -= vec.u;
            v -= vec.v;
            w -= vec.w;
            return *this;
        }

        constexpr vec3 operator+=(const vec3 &vec) {
            u += vec.u;
            v += vec.v;
            w += vec.w;
            return *this;
        }

        constexpr vec3 operator-(const vec3 &vec) const { return vec3(*this) -= vec; }

        constexpr vec3 operator+(const vec3 &vec) const { return vec3(*this) += vec; }

        vec3 operator+(const p3 &p) const;

        vec3 operator-(const p3 &p) const;
        constexpr double square() const { return *this * *this; }

        constexpr vec3 cross(const vec3& rhs) const {
            auto res = vec3();
            res.u = v * rhs.w - w * rhs.v;
            res.v = w * rhs.u - u * rhs.w;
            res.w = u * rhs.v - v * rhs.u;
            return res;
        }

        constexpr vec3 operator-() const { return vec3(-u, -v, -w); }

        constexpr vec3 operator-(double f) const { return vec3(u - f, v - f, w - f); };

        p3 p() const;

        constexpr double norm() const { return std::sqrt(square()); }

        inline vec3 normalize() const { return *this / norm(); };

        constexpr vec3 rotateX(double angle) const {
            auto vec = vec3();
            vec.u = u;
            vec.v = this->dot(vec3(0, cos(angle), -sin(angle)));
            vec.w = this->dot(vec3(0, sin(angle), cos(angle)));
            return vec;
        }
        constexpr vec3 rotateY(double angle) const {
            return vec3(this->dot(vec3(cos(angle), 0, sin(angle))), v, this->dot(vec3(-sin(angle), 0, cos(angle))));
        }
    };

    struct p3 {
        double x;
        double y;
        double z;

        constexpr p3(double i, double j, double k) : x(i), y(j), z(k) {};
        p3() = default;

        inline double dist(const p3& p) const {
            return (p - *this).norm();
        }

        constexpr vec3 operator-(const p3& p) const {
            return vec3(x - p.x, y - p.y, z - p.z);
        }

        constexpr p3 operator+=(double d) {
            x += d;
            y += d;
            z += d;
            return *this;
        }

        constexpr p3 operator-=(double d) {
            x -= d;
            y -= d;
            z -= d;
            return *this;
        }

        constexpr p3 operator+(double d) const { return p3(x + d, y + d, z + d); }
        constexpr p3 operator-(double d) const { return p3(x - d, y - d, z - d); }

        constexpr p3 operator+(const p3& p) const {
            return p3(x + p.x, y + p.y, z + p.z);
        }

        constexpr p3 operator+(const vec3& p) const {
            return p3(x + p.u, y + p.v, z + p.w);
        }

        constexpr p3 operator-(const vec3& p) const { return p3(x - p.u, y - p.v, z - p.w); }

        constexpr p3 operator-() const {
            return p3(-x, -y, -z);
        }

        constexpr vec3 vec() const {
            return vec3(x, y, z);
        }

    };

}

std::ostream &operator<<(std::ostream &out, const raytracing::color &c);
std::ostream &operator<<(std::ostream &out, const raytracing::vec3 &v);

raytracing::vec3 operator*(double, const raytracing::vec3&);

#endif //RAYTRACING_TYPES_H

