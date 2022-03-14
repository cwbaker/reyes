#ifndef REYES_CONDITIONMASK_HPP_INCLUDED
#define REYES_CONDITIONMASK_HPP_INCLUDED

#include <vector>

namespace reyes
{

class ConditionMask
{
    std::vector<unsigned char> mask_; ///< The mask that specifies whether or not an element is to be processed.
    int processed_; ///< The number of elements that are to be processed by this mask.

public:
    ConditionMask();
    const std::vector<unsigned char>& mask() const;
    int processed() const;
    bool empty() const;
    void generate( const float* values, int length );
    void generate( const ConditionMask& condition_mask, const float* values, int length );
    void invert();
};

}

#endif
