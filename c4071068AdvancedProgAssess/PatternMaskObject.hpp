#include "Pattern.hpp"

template <size_t T>
class PatternMaskObject
{
private:
    const PatternMask patterns[T];

public:
    PatternMaskObject(const PatternMask(&patterns)[T])
    {
        for (size_t i = 0; i < T; i++) {
            this->patterns[i] = patterns[i];
        }
    }

    int GetPatternSize() const
    {
        return T;
    }

    PatternMask GetPatternMask(int i) {
        return this->patterns[i];
    }
};
