#pragma once

struct rect_legacy
{
    sdk::vector2d vec_min;
    sdk::vector2d vec_max;

    // Constructor to initialize with two sdk::vector2d for min and max
    rect_legacy(const sdk::vector2d& min, const sdk::vector2d& max)
        : vec_min(min), vec_max(max) {}

    // Constructor to initialize with a top-left position (min) and width, height
    rect_legacy(const sdk::vector2d& min_size, float width, float height)
    {
        vec_min = min_size;
        vec_max = sdk::vector2d{ min_size.x + width, min_size.y + height };
    }

    // Function to adjust the box size with padding
    rect_legacy adjust_for_padding(float padding_x, float padding_y) const
    {
        sdk::vector2d adjusted_min = vec_min + sdk::vector2d{ padding_x, padding_y };
        sdk::vector2d adjusted_max = vec_max - sdk::vector2d{ padding_x, padding_y };
        return rect_legacy{ adjusted_min, adjusted_max };
    }

    // Function to get the size of the rectangle
    sdk::vector2d size() const
    {
        return vec_max - vec_min;
    }
};
