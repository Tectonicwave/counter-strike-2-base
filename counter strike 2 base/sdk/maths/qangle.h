#pragma once

#include <algorithm> // For std::clamp
#include <cmath>     // For std::remainder
#include "intrinsic.h"

namespace sdk
{

    class qangle
    {
    public:
        float pitch{}, yaw{}, roll{};

        __forceinline qangle() :
            qangle(0.f, 0.f, 0.f) {}

        __forceinline qangle(const float pitch, const float yaw, const float roll) :
            pitch(pitch), yaw(yaw), roll(roll) {}

        __forceinline qangle operator+(const qangle& other) const { return qangle(pitch + other.pitch, yaw + other.yaw, roll + other.roll); }

        __forceinline qangle operator-(const qangle& other) const { return qangle(pitch - other.pitch, yaw - other.yaw, roll - other.roll); }

        __forceinline qangle operator*(const float scalar) const { return qangle(pitch * scalar, yaw * scalar, roll * scalar); }

        __forceinline qangle operator/(const float scalar) const { return qangle(pitch / scalar, yaw / scalar, roll / scalar); }

        __forceinline qangle operator+=(const qangle& other)
        {
            pitch += other.pitch;
            yaw += other.yaw;
            roll += other.roll;
            return *this;
        }

        __forceinline qangle operator-=(const qangle& other)
        {
            pitch -= other.pitch;
            yaw -= other.yaw;
            roll -= other.roll;
            return *this;
        }

        __forceinline qangle operator*=(const float scalar)
        {
            pitch *= scalar;
            yaw *= scalar;
            roll *= scalar;
            return *this;
        }

        __forceinline qangle operator/=(const float scalar)
        {
            pitch /= scalar;
            yaw /= scalar;
            roll /= scalar;
            return *this;
        }

        __forceinline bool operator==(const qangle& other) const { return pitch == other.pitch && yaw == other.yaw && roll == other.roll; }

        __forceinline bool operator!=(const qangle& other) const { return pitch != other.pitch || yaw != other.yaw || roll != other.roll; }

        __forceinline bool is_zero(float tolerance = 0.01f) const
        {
            return (pitch > -tolerance && pitch < tolerance && yaw > -tolerance && yaw < tolerance && roll > -tolerance && roll < tolerance);
        }

        // @returns : true if each component of angle is finite, false otherwise
        __forceinline bool is_valid() const
        {
            return (std::isfinite(this->pitch) && std::isfinite(this->yaw) && std::isfinite(this->roll));
        }

        __forceinline qangle clamp()
        {
            pitch = std::clamp(pitch, -89.f, 89.f);
            yaw = std::clamp(std::remainder(yaw, 360.f), -180.f, 180.f);
            roll = 0.f;  // Roll is usually clamped to 0 in many cases (e.g., camera angles in games).
            return *this;
        }

        __forceinline float length() const { return sqrt_ps(pitch * pitch + yaw * yaw + roll * roll); }

        __forceinline float length_sqr() const { return pitch * pitch + yaw * yaw + roll * roll; }
    };

}
