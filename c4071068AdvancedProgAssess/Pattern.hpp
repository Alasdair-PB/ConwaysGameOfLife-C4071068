#pragma once
#include "Vector2.hpp"
#include <bitset>

template <size_t T>
class PatternMask
{

public:
    const std::bitset<T> pattern;
    const Vector2<int> dimensions;
    PatternMask(const std::bitset<T>& pattern, const Vector2<int>& dimensions)
        : pattern(pattern), dimensions(dimensions) {}

    const Vector2<int> GetDimensions() const {
        return dimensions;
    }

    const std::bitset<T>& GetPattern() const {
        return pattern;
    }
};
