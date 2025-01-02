#pragma once

#include <algorithm> // For std::clamp
#include <cmath>     // For std::remainder
#include "intrinsic.h"

namespace sdk
{
    class vector2d
    {
    public:
        float x{}, y{};

        __forceinline vector2d() :
            vector2d(0.f, 0.f) {}

        __forceinline vector2d(const float x, const float y) :
            x(x), y(y) {}

        __forceinline vector2d operator+(const vector2d& v) const { return vector2d(x + v.x, y + v.y); }

        __forceinline vector2d operator-(const vector2d& v) const { return vector2d(x - v.x, y - v.y); }

        __forceinline vector2d operator*(const float v) const { return vector2d(x * v, y * v); }

        __forceinline vector2d operator/(const float v) const { return vector2d(x / v, y / v); }

        __forceinline vector2d operator+=(const vector2d& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        __forceinline vector2d operator-=(const vector2d& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        __forceinline vector2d operator*=(const float other)
        {
            x *= other;
            y *= other;
            return *this;
        }

        __forceinline vector2d operator/=(const float other)
        {
            x /= other;
            y /= other;
            return *this;
        }

        __forceinline bool operator==(const vector2d& other) const { return x == other.x && y == other.y; }

        __forceinline bool operator!=(const vector2d& other) const { return x != other.x || y != other.y; }

        __forceinline float dot(const vector2d& other) const { return x * other.x + y * other.y; }

        __forceinline float length() const { return sqrt_ps(x * x + y * y); }

        __forceinline float length_sqr() const { return this->x * this->x + this->y * this->y; }

        __forceinline vector2d normalize()
        {
            const auto l = length();

            if (l > 0)
            {
                x /= l;
                y /= l;
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
            }
            return l;
        }

        vector2d round() const { return { std::round(x), std::round(y) }; }

        __forceinline vector2d normalized() const
        {
            auto norm = *this;
            norm.normalize();
            return norm;
        }

        __forceinline bool is_zero(float tolerance = 0.01f) const
        {
            return (x > -tolerance && x < tolerance && y > -tolerance && y < tolerance);
        }
    };

}
