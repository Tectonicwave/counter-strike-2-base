#pragma once
#include <imgui.h>

namespace sdk
{
    struct color {
    public:
        float r, g, b, a;

        ImU32 to_u32() const {
            return IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), (int)(a * 255));
        }

        color lerp(const color& other, float t) const {
            return color{
                r + t * (other.r - r),
                g + t * (other.g - g),
                b + t * (other.b - b),
                a + t * (other.a - a)
            };

        }
    };

}