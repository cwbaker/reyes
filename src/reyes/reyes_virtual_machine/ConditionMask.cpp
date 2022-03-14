//
// ConditionMask.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ConditionMask.hpp"
#include <reyes/assert.hpp>

using namespace reyes;

ConditionMask::ConditionMask()
: mask_(),
  processed_( 0 )
{
}

const std::vector<unsigned char>& ConditionMask::mask() const
{
    return mask_;
}

int ConditionMask::processed() const
{
    return processed_;
}

bool ConditionMask::empty() const
{
    return processed_ == 0;
}

void ConditionMask::generate( const float* values, int length )
{
    REYES_ASSERT( values );
    REYES_ASSERT( length >= 0 );
    
    mask_.insert( mask_.end(), length, 0 );
    int processed = 0;
    unsigned char* mask = &mask_[0];
    for ( int i = 0; i < length; ++i )
    {
        bool process = values[i] != 0.0f;
        mask[i] = process;
        processed += process ? 1 : 0;
    }
    processed_ = processed;
}

void ConditionMask::generate( const ConditionMask& condition_mask, const float* values, int length )
{
    REYES_ASSERT( values );
    REYES_ASSERT( length >= 0 );
    REYES_ASSERT( condition_mask.mask_.size() == length );
    
    const unsigned char* existing_mask = &condition_mask.mask_[0];

    mask_.insert( mask_.end(), length, 0 );
    int processed = 0;
    unsigned char* mask = &mask_[0];
    for ( int i = 0; i < length; ++i )
    {
        bool process = values[i] != 0.0f && existing_mask[i];
        mask[i] = process;
        processed += process ? 1 : 0;
    }
    processed_ = processed;
}

void ConditionMask::invert()
{
    const int size = int(mask_.size());
    unsigned char* mask = &mask_[0];
    
    processed_ = 0;
    for ( int i = 0; i < size; ++i )
    {
        int process = !mask[i];
        mask[i] = process != 0;
        processed_ += process != 0;
    }
}
