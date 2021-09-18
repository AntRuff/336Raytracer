#ifndef VEC_H
# define VEC_H

#include <cmath>
#include <iostream>

class vec3 {
private:
    float v[3];

public:
    //Constructor/destructor
    vec3(float x, float y, float z) { v[0] = x; v[1] = y; v[2] = z; }
    vec3(const vec3 &in) { v[0] = in.v[0]; v[1] = in.v[1]; v[2] = in.v[2]; }
    vec3() { v[0] = v[1] = v[2] = 0; }
    ~vec3() {}

    //Accessors (position and color)
    float x() { return v[0]; }
    float y() { return v[1]; }
    float z() { return v[2]; }

    float r() { return v[0]; }
    float g() { return v[1]; }
    float b() { return v[2]; }

    //Unary operators
    vec3 operator-() { return vec3(-v[0], -v[1], -v[2]); }
    const vec3 &operator+() { return *this; }

    //Array syntax
    float operator[](int i) const { return v[i]; }
    float &operator[](int i) { return v[i]; }

    //Arithmetic
    vec3 &operator+=(const vec3 &in);
    vec3 &operator-=(const vec3 &in);
    vec3 &operator*=(const vec3 &in);
    vec3 &operator/=(const vec3 &in);
    vec3 &operator*=(const float f);
    vec3 &operator/=(const float f);
    vec3 &operator=(const vec3 &in);

    vec3 operator+(const vec3 &in) const;
    vec3 operator-(const vec3 &in) const;
    vec3 operator*(const vec3 &in) const;
    vec3 operator/(const vec3 &in) const;

    friend vec3 operator*(float f, const vec3 &v1);
    vec3 operator*(float f);
    vec3 operator/(float f);

    float dot(const vec3 &in);
    vec3 cross(const vec3 &in) const;

    //Vector operations
    float length() const { return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }
    float squared_length() const { return (v[0] * v[0] +
                                           v[1] * v[1] +
                                           v[2] * v[2]); }
    vec3 &to_unit();

    vec3 unit() const;

    //I/O
    friend std::ostream &operator<<(std::ostream &o, const vec3 &v);
    friend std::istream &operator>>(std::istream &i, vec3 &v);
};

#endif