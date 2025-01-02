#pragma once

#pragma
#include <algorithm> // For std::clamp
#include <cmath>     // For std::remainder
#include "intrinsic.h"

namespace sdk
{
    class vector4d
    {
    public:
        float x{}, y{}, z{}, w{};

        __forceinline vector4d() :
            vector4d(0.f, 0.f, 0.f, 0.f) {}

        __forceinline vector4d(const float x, const float y, const float z, const float w) :
            x(x), y(y), z(z), w(w) {}

        __forceinline vector4d operator+(const vector4d& v) const { return vector4d(x + v.x, y + v.y, z + v.z, w + v.w); }

        __forceinline vector4d operator-(const vector4d& v) const { return vector4d(x - v.x, y - v.y, z - v.z, w - v.w); }

        __forceinline vector4d operator*(const float v) const { return vector4d(x * v, y * v, z * v, w * v); }

        __forceinline vector4d operator/(const float v) const { return vector4d(x / v, y / v, z / v, w / v); }

        __forceinline vector4d operator+=(const vector4d& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        __forceinline vector4d operator-=(const vector4d& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        __forceinline vector4d operator*=(const float other)
        {
            x *= other;
            y *= other;
            z *= other;
            w *= other;
            return *this;
        }

        __forceinline vector4d operator/=(const float other)
        {
            x /= other;
            y /= other;
            z /= other;
            w /= other;
            return *this;
        }

        __forceinline bool operator==(const vector4d& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }

        __forceinline bool operator!=(const vector4d& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

        __forceinline float dot(const vector4d& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }

        __forceinline float length() const { return sqrt_ps(x * x + y * y + z * z + w * w); }

        __forceinline float length_sqr() const { return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w; }

        __forceinline vector4d normalize()
        {
            const auto l = length();

            if (l > 0)
            {
                x /= l;
                y /= l;
                z /= l;
                w /= l;
            }

            return *this;
        }

        __forceinline float normalize_in_place()
        {
            const auto l = length();
            if (l > 0)
            {
                x /= l;
                y /= l;
                z /= l;
                w /= l;
            }
            return l;
        }

        __forceinline vector4d normalized() const
        {
            auto norm = *this;
            norm.normalize();
            return norm;
        }

        __forceinline bool is_zero(float tolerance = 0.01f) const
        {
            return (x > -tolerance && x < tolerance && y > -tolerance && y < tolerance && z > -tolerance && z < tolerance && w > -tolerance && w < tolerance);
        }
    };

    using quaternion_storage = vector4d; // not sure about this one
    using quaternion = vector4d;
    using fltx4 = vector4d;

}
