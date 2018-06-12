//
// evaluate.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "evaluate.hpp"
#include "Instruction.hpp"
#include <sweet/fx/ProgramData.hpp>
#include <sweet/math/scalar.ipp>
#include <sweet/assert/assert.hpp>
#include <algorithm>

using std::max;
using namespace sweet::math;
using namespace sweet::fx;

namespace sweet
{

namespace fx
{

void evaluate_u1( const ProgramCurve* curve, float* result, const float* parameter )
{
    const float rate = curve->rate;
    const float length = curve->length;
    const float* values = curve->values.get();
    const float xx = clamp( parameter[0], 0.0f, length );
    int i0 = int(xx * rate);
    int i1 = i0 + 1;
    float t0 = i0 / rate;
    float t1 = i1 / rate;
    float t = clamp( (xx - t0) / (t1 - t0), 0.0f, 1.0f );

    const int elements = 1;
    int frame = i0 * elements;
    int next_frame = i1 * elements;
    result[0] = lerp( values[frame + 0], values[next_frame + 0], t );
}

void evaluate_u2( const ProgramCurve* curve, float* result, const float* parameter )
{
    const float rate = curve->rate;
    const float length = curve->length;
    const float* values = curve->values.get();
    const float xx = clamp( parameter[0], 0.0f, length );
    int i0 = int(xx * rate);
    int i1 = i0 + 1;
    float t0 = i0 / rate;
    float t1 = i1 / rate;
    float t = clamp( (xx - t0) / (t1 - t0), 0.0f, 1.0f );

    const int elements = 2;
    int frame = i0 * elements;
    int next_frame = i1 * elements;
    result[0] = lerp( values[frame + 0], values[next_frame + 0], t );
    result[1] = lerp( values[frame + 1], values[next_frame + 1], t );
}

void evaluate_u3( const ProgramCurve* curve, float* result, const float* parameter )
{
    const float rate = curve->rate;
    const float length = curve->length;
    const float* values = curve->values.get();
    const float xx = clamp( parameter[0], 0.0f, length );
    int i0 = int(xx * rate);
    int i1 = i0 + 1;
    float t0 = i0 / rate;
    float t1 = i1 / rate;
    float t = clamp( (xx - t0) / (t1 - t0), 0.0f, 1.0f );

    const int elements = 3;
    int frame = i0 * elements;
    int next_frame = i1 * elements;
    result[0] = lerp( values[frame + 0], values[next_frame + 0], t );
    result[1] = lerp( values[frame + 1], values[next_frame + 1], t );
    result[2] = lerp( values[frame + 2], values[next_frame + 2], t );
}

#ifdef BUILD_OS_ANDROID
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "TAG", __VA_ARGS__);
#endif

void evaluate_u4( const ProgramCurve* curve, float* result, const float* parameter )
{
    const float rate = curve->rate;
    const float length = curve->length;
    const float* values = curve->values.get();
    const float xx = clamp( parameter[0], 0.0f, length );
    int i0 = int(xx * rate);
    int i1 = i0 + 1;
    float t0 = i0 / rate;
    float t1 = i1 / rate;
    float t = clamp( (xx - t0) / (t1 - t0), 0.0f, 1.0f );

    const int elements = 4;
    int frame = i0 * elements;
    int next_frame = i1 * elements;
    result[0] = lerp( values[frame + 0], values[next_frame + 0], t );
    result[1] = lerp( values[frame + 1], values[next_frame + 1], t );
    result[2] = lerp( values[frame + 2], values[next_frame + 2], t );
    result[3] = lerp( values[frame + 3], values[next_frame + 3], t );
}

void evaluate_v1( const ProgramCurve* curve, float* result, const float* parameter, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        evaluate_u1( curve, &result[i], &parameter[i] );
    }
}

void evaluate_v2( const ProgramCurve* curve, float* result, const float* parameter, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        const int elements = 2;
        evaluate_u2( curve, &result[i * elements], &parameter[i] );
    }
}

void evaluate_v3( const ProgramCurve* curve, float* result, const float* parameter, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        const int elements = 3;
        evaluate_u3( curve, &result[i * elements], &parameter[i] );
    }
}

void evaluate_v4( const ProgramCurve* curve, float* result, const float* parameter, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        const int elements = 4;
        evaluate_u4( curve, &result[i * elements], &parameter[i] );
    }
}

void evaluate( int dispatch, float* result, const float* parameter, int length, const ProgramCurve* curve )
{
    SWEET_ASSERT( curve );
    SWEET_ASSERT( result );
    SWEET_ASSERT( parameter );

    switch ( dispatch )
    {
        case INSTRUCTION_U1:
            evaluate_u1( curve, result, parameter );
            break;

        case INSTRUCTION_U2:
            evaluate_u2( curve, result, parameter );
            break;

        case INSTRUCTION_U3:
            evaluate_u3( curve, result, parameter );
            break;

        case INSTRUCTION_U4:
            evaluate_u4( curve, result, parameter );
            break;

        case INSTRUCTION_V1:
            evaluate_v1( curve, result, parameter, length );
            break;

        case INSTRUCTION_V2:
            evaluate_v2( curve, result, parameter, length );
            break;

        case INSTRUCTION_V3:
            evaluate_v3( curve, result, parameter, length );
            break;

        case INSTRUCTION_V4:
            evaluate_v4( curve, result, parameter, length );
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }
}

}

}
