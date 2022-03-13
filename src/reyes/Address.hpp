#pragma once

#include "Segment.hpp"

namespace reyes
{

class Address
{
    int segment_ : 8;
    int offset_ : 24;

public:
    Address();
    Address( int value );
    Address( Segment segment, int address );

    Segment segment() const;
    int offset() const;
    int value() const;

    void set_segment( Segment segment );
    void set_offset( int offset );
};

}