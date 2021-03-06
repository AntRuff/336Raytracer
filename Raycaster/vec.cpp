#include "vec.h"

double vec3::dot(const vec3 &in)
{
    return v[0] * in.v[0] + v[1] * in.v[1] + v[2] * in.v[2];
}

vec3 vec3::cross(const vec3 &in) const
{
    return vec3(v[1] * in.v[2] - v[2] * in.v[1], v[2] * in.v[0] - v[0] * in.v[2], v[0] * in.v[1] - v[1] * in.v[0]);
}

std::ostream &operator<<(std::ostream &o, const vec3 &v)
{
    return o << v.v[0] << " " << v.v[1] << " " << v.v[2];
}

std::istream &operator>>(std::istream &i, vec3 &v)
{
    return i >> v.v[0] >> v.v[1] >> v.v[2];
}

vec3 &vec3::operator+=(const vec3 &in)
{
    v[0] += in.v[0]; v[1] += in.v[1]; v[2] += in.v[2];
    return *this;
}

vec3 &vec3::operator-=(const vec3 &in)
{
    v[0] -= in.v[0]; v[1] -= in.v[1]; v[2] -= in.v[2];
    return *this;
}

vec3 &vec3::operator*=(const vec3 &in)
{
    cross(in);
    return *this;
}

vec3 &vec3::operator/=(const vec3 &in)
{
    throw std::logic_error("Vector cannot be divided by another vector.");
}

vec3 &vec3::operator*=(const double f)
{
    v[0] *= f; v[1] *= f; v[2] *= f;
    return *this;
}

vec3 &vec3::operator/=(const double f)
{
    v[0] /= f; v[1] /= f; v[2] /= f;
    return *this;
}

vec3 &vec3::operator=(const vec3 &in)
{
    v[0] = in.v[0]; v[1] = in.v[1]; v[2] = in.v[2];
    return *this;
}

vec3 vec3::operator+(const vec3 &in) const
{
    return vec3(v[0] + in.v[0], v[1] + in.v[1], v[2] + in.v[2]);
}

vec3 vec3::operator-(const vec3 &in) const
{
    return vec3(v[0] - in.v[0], v[1] - in.v[1], v[2] - in.v[2]);
}

vec3 vec3::operator*(const vec3 &in) const
{
//    return cross(in);
    return vec3(v[0] * in.v[0], v[1] * in.v[1], v[2] * in.v[2]);
}

vec3 vec3::operator/(const vec3 &in) const
{
    throw std::logic_error("Vector cannot be divided by another vector.");
}

vec3 operator*(double f, const vec3 &v1)
{
    return vec3(v1.v[0] * f, v1.v[1] * f, v1.v[2] * f);
}

vec3 vec3::operator*(double f)
{
    return vec3(v[0] * f, v[1] * f, v[2] * f);
}

vec3 vec3::operator/(double f)
{
    return vec3(v[0] / f, v[1] / f, v[2] / f);
}

double vec3::length() {
    return sqrt(pow(x(), 2) + pow(y(), 2) + pow(z(), 2));
}
double vec3::length(vec3 origin) {
    return sqrt(pow(x() - origin.x(), 2) + pow(y() - origin.y(), 2) + pow(z() - origin.z(), 2));
}
//vec3 &vec3::to_unit()
//{
//
//}

//vec3 vec3::unit() const
//{
//}
