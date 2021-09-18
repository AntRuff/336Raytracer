#include "vec.h"

float vec3::dot(const vec3 &in)
{
    return v[0] * in.v[0] + v[1] * in.v[1] + v[2] * in.v[2];
}

vec3 vec3::cross(const vec3 &in) const
{
    return vec3(v[1] * in.v[2] - v[2] * in.v[1],
                v[2] * in.v[0] - v[0] * in.v[2],
                v[0] * in.v[1] - v[1] * in.v[0]);
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
    return vec3(v[0] + in.v[0], v[1] + in.v[1], v[2] + in.v[2]);
}

vec3 &vec3::operator-=(const vec3 &in)
{
    return vec3(v[0] - in.v[0], v[1] - in.v[1], v[2] - in.v[2]);
}

vec3 &vec3::operator*=(const vec3 &in)
{
}

vec3 &vec3::operator/=(const vec3 &in)
{
}

vec3 &vec3::operator*=(const float f)
{
}

vec3 &vec3::operator/=(const float f)
{
}

vec3 &vec3::operator=(const vec3 &in)
{
}

vec3 vec3::operator+(const vec3 &in) const
{
}

vec3 vec3::operator-(const vec3 &in) const
{
}

vec3 vec3::operator*(const vec3 &in) const
{
}

vec3 vec3::operator/(const vec3 &in) const
{
}

vec3 operator*(float f, const vec3 &v1)
{
}

vec3 vec3::operator*(float f)
{
}

vec3 vec3::operator/(float f)
{
}

vec3 &vec3::to_unit()
{
}

vec3 vec3::unit() const
{
}