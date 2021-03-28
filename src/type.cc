//
// Created by simon on 2/23/21.
//

#include "type.hh"
using namespace raytracing;

std::ostream& operator<<(std::ostream& out, const raytracing::color& c) {
    out << "{ r: " << c.r << ", g: " << c.g << ", b: " << c.b << " }";
    return out;
}

std::ostream& operator<<(std::ostream& out, const raytracing::vec3& v) {
    out << "{ u: " << v.u << ", v: " << v.v << ", w: " << v.w << " }";
    return out;
}

vec3 operator*(double f, const vec3& v) {
    return v * f;
}

vec3 vec3::operator+(const p3 &p) const {
    return vec3(this->u + p.x, this->v + p.y, this->w + p.z);
}

vec3 vec3::operator-(const p3 &p) const {
    return vec3(this->u - p.x, this->v - p.y, this->w - p.z);
}

constexpr p3 vec3::p() const {
    return p3(u, v, w);
}