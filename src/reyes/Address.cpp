
#include "Address.hpp"
#include <assert.h>

using namespace reyes;

Address::Address()
: segment_( SEGMENT_NULL )
, offset_( 0 )
{
}

Address::Address( int value )
: segment_( value >> 24 )
, offset_( value & 0x00ffffff )
{
}

Address::Address( Segment segment, int offset )
: segment_( static_cast<int>(segment) )
, offset_( offset )
{
}

Segment Address::segment() const
{
    return static_cast<Segment>( segment_ );
}

int Address::offset() const
{
    return offset_;
}

int Address::value() const
{
    return segment_ << 24 | offset_;
}

void Address::set_segment( Segment segment )
{
    segment_ = segment;
}

void Address::set_offset( int offset )
{
    offset_ = offset;
}
