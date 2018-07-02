//
// ConditionMask.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ConditionMask.hpp"
#include <reyes/Value.hpp>
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

void ConditionMask::generate( std::shared_ptr<Value> value )
{
    REYES_ASSERT( value );
    REYES_ASSERT( value->type() == TYPE_INTEGER );
    
    const unsigned int size = value->size();
    const int* values = value->int_values();

    mask_.insert( mask_.end(), size, 0 );
    processed_ = 0;
    unsigned char* mask = &mask_[0];
    for ( unsigned int i = 0; i < size; ++i )
    {
        int process = values[i];
        mask[i] = process != 0;
        processed_ += process != 0;
    }
}

void ConditionMask::generate( const ConditionMask& condition_mask, std::shared_ptr<Value> value )
{
    REYES_ASSERT( value );
    REYES_ASSERT( value->type() == TYPE_INTEGER );
    REYES_ASSERT( condition_mask.mask_.size() == value->size() );
    
    const unsigned int size = value->size();
    const int* values = value->int_values();
    const unsigned char* existing_mask = &condition_mask.mask_[0];

    mask_.insert( mask_.end(), size, 0 );
    processed_ = 0;
    unsigned char* mask = &mask_[0];
    for ( unsigned int i = 0; i < size; ++i )
    {
        int process = values[i] && existing_mask[i];
        mask[i] = process != 0;
        processed_ += process != 0;
    }
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
