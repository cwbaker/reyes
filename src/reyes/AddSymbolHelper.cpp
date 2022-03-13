//
// AddSymbolHelper.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "AddSymbolHelper.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "assert.hpp"
#include <reyes/reyes_virtual_machine/mathematical_functions.hpp>
#include <reyes/reyes_virtual_machine/geometric_functions.hpp>
#include <reyes/reyes_virtual_machine/color_functions.hpp>
#include <reyes/reyes_virtual_machine/matrix_functions.hpp>
#include <reyes/reyes_virtual_machine/shading_and_lighting_functions.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace reyes;

AddSymbolHelper::AddSymbolHelper( SymbolTable* symbol_table, Segment segment )
: symbol_table_( symbol_table )
, segment_( segment )
, symbol_()
{   
    REYES_ASSERT( symbol_table_ );
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );    
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_segment( segment_ );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, int, void**), ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_ );
    symbol_->add_parameter( type, storage );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, float value )
{
    REYES_ASSERT( symbol_table_ );
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( TYPE_FLOAT );
    symbol_->set_storage( STORAGE_CONSTANT );
    symbol_->set_value( value );
    symbol_->set_segment( segment_ );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::add_symbols( Segment segment )
{
    segment_ = segment;
    return *this;
}

AddSymbolHelper& AddSymbolHelper::add_default_symbols()
{
    add_symbols( SEGMENT_CONSTANT )
        ( "PI", float(M_PI) )
    ;

    add_symbols()
        ( "radians", &radians, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "radians", &radians, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )        
        ( "degrees", &degrees, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "degrees", &degrees, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
    ;
    
    add_symbols()
        ( "sin", &sin, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "sin", &sin, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "asin", &asin, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "asin", &asin, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "cos", &cos, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "cos", &cos, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "acos", &acos, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "acos", &acos, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "tan", &tan, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "tan", &tan, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "atan", &atan, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "atan", &atan, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "atan", &atan2, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "atan", &atan2, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
    ;
    
    add_symbols()
        ( "pow", &pow, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "pow", &pow, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
        ( "exp", &exp, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "exp", &exp, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "sqrt", &sqrt, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "sqrt", &sqrt, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "inversesqrt", &inversesqrt, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "inversesqrt", &inversesqrt, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "log", &pow, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "log", &pow, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
    ;

    add_symbols()
        ( "mod", &mod, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "mod", &mod, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
        ( "abs", &abs, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "abs", &abs, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "sign", &sign, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "sign", &sign, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
    ;
    
    add_symbols()
        ( "mix", &mix_float, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "mix", &mix_float, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
        ( "mix", &mix_vec3, TYPE_POINT, STORAGE_UNIFORM )
            ( TYPE_POINT, STORAGE_UNIFORM )( TYPE_POINT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "mix", &mix_vec3, TYPE_POINT, STORAGE_VARYING )
            ( TYPE_POINT, STORAGE_VARYING )( TYPE_POINT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
    ;
    
    add_symbols()
        ( "floor", &floor, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "floor", &floor, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
        ( "ceil", &ceil, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "ceil", &ceil, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
        ( "round", &round, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "round", &round, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_VARYING )
    ;

    add_symbols()
        ( "step", &step, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "step", &step, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_VARYING )
        ( "smoothstep", &smoothstep, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "smoothstep", &smoothstep, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_VARYING )
    ;
    
    add_symbols()
        ( "Du", &du_float, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT )
        ( "Du", &du_vec3, TYPE_COLOR, STORAGE_VARYING )
            ( TYPE_COLOR )
        ( "Du", &du_vec3, TYPE_VECTOR, STORAGE_VARYING )
            ( TYPE_POINT )
        ( "Du", &du_vec3, TYPE_VECTOR, STORAGE_VARYING )
            ( TYPE_VECTOR )
        ( "Dv", &du_float, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT )
        ( "Dv", &du_vec3, TYPE_COLOR, STORAGE_VARYING )
            ( TYPE_COLOR )
        ( "Dv", &du_vec3, TYPE_VECTOR, STORAGE_VARYING )
            ( TYPE_POINT )
        ( "Dv", &du_vec3, TYPE_VECTOR, STORAGE_VARYING )
            ( TYPE_VECTOR )
        ( "Deriv", &deriv_float, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_FLOAT )( TYPE_FLOAT )
        ( "Deriv", &deriv_vec3, TYPE_COLOR, STORAGE_VARYING )
            ( TYPE_COLOR )( TYPE_FLOAT )
        ( "Deriv", &deriv_vec3, TYPE_POINT, STORAGE_VARYING )
            ( TYPE_POINT )( TYPE_FLOAT )
        ( "Deriv", &deriv_vec3, TYPE_VECTOR, STORAGE_VARYING )
            ( TYPE_VECTOR )( TYPE_FLOAT )
    ;            
        
    add_symbols()
        ( "random", &uniform_float_random, TYPE_FLOAT, STORAGE_UNIFORM )            
        ( "random", &uniform_vec3_random, TYPE_POINT, STORAGE_UNIFORM )            
        ( "random", &uniform_vec3_random, TYPE_COLOR, STORAGE_UNIFORM )            
        ( "random", &float_random, TYPE_FLOAT, STORAGE_VARYING )            
        ( "random", &vec3_random, TYPE_POINT, STORAGE_VARYING )            
        ( "random", &vec3_random, TYPE_COLOR, STORAGE_VARYING )            
    ;
        
    add_symbols()
        ( "xcomp", &xcomp, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_POINT, STORAGE_UNIFORM )
        ( "xcomp", &xcomp, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_POINT, STORAGE_VARYING )
        ( "ycomp", &ycomp, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_POINT, STORAGE_UNIFORM )
        ( "ycomp", &ycomp, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_POINT, STORAGE_VARYING )
        ( "zcomp", &zcomp, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_POINT, STORAGE_UNIFORM )
        ( "zcomp", &zcomp, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_POINT, STORAGE_VARYING )
        ( "setxcomp", &setxcomp, TYPE_NULL )
            ( TYPE_POINT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "setxcomp", &setxcomp, TYPE_NULL )
            ( TYPE_POINT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
        ( "setycomp", &setycomp, TYPE_NULL )
            ( TYPE_POINT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "setycomp", &setycomp, TYPE_NULL )
            ( TYPE_POINT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
        ( "setzcomp", &setzcomp, TYPE_NULL )
            ( TYPE_POINT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "setzcomp", &setzcomp, TYPE_NULL )
            ( TYPE_POINT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
        ( "length", &length, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_VECTOR, STORAGE_UNIFORM )
        ( "length", &length, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_VECTOR, STORAGE_VARYING )
        ( "normalize", &normalize, TYPE_VECTOR )
            ( TYPE_VECTOR )
        ( "distance", &distance, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_POINT, STORAGE_UNIFORM )( TYPE_POINT, STORAGE_UNIFORM )
        ( "distance", &distance, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_POINT, STORAGE_VARYING )( TYPE_POINT, STORAGE_VARYING )
        ( "rotate", &rotate, TYPE_VECTOR, STORAGE_UNIFORM )
            ( TYPE_POINT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_POINT, STORAGE_UNIFORM )( TYPE_POINT, STORAGE_UNIFORM )
        ( "rotate", &rotate, TYPE_VECTOR, STORAGE_VARYING )
            ( TYPE_POINT, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_POINT, STORAGE_UNIFORM )( TYPE_POINT, STORAGE_UNIFORM )
        ( "area", &area, TYPE_FLOAT, STORAGE_VARYING )
            ( TYPE_POINT )
        ( "faceforward", &faceforward_vv, TYPE_VECTOR )
            ( TYPE_VECTOR )( TYPE_VECTOR )
        ( "faceforward", &faceforward_vvv, TYPE_VECTOR )
            ( TYPE_VECTOR )( TYPE_VECTOR )( TYPE_VECTOR )
        ( "reflect", &reflect, TYPE_VECTOR )
            ( TYPE_VECTOR )( TYPE_VECTOR )
        ( "refract", &reflect, TYPE_VECTOR )
            ( TYPE_VECTOR )( TYPE_VECTOR )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "fresnel", &fresnel, TYPE_NULL )
            ( TYPE_VECTOR )( TYPE_VECTOR )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT )( TYPE_FLOAT )
        ( "transform", &transform_sv, TYPE_POINT )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_POINT )
        ( "transform", &transform_ssv, TYPE_POINT )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_STRING, STORAGE_UNIFORM )( TYPE_POINT )
        ( "transform", &transform_mv, TYPE_POINT )
            ( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_POINT )
        ( "transform", &transform_smv, TYPE_POINT )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_POINT )
        ( "vtransform", &vtransform_sv, TYPE_POINT )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_POINT )
        ( "vtransform", &vtransform_ssv, TYPE_POINT )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_STRING, STORAGE_UNIFORM )( TYPE_POINT )
        ( "vtransform", &vtransform_mv, TYPE_POINT )
            ( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_POINT )
        ( "vtransform", &vtransform_smv, TYPE_POINT )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_POINT )
        ( "ntransform", &ntransform_sv, TYPE_POINT )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_POINT )
        ( "ntransform", &ntransform_ssv, TYPE_POINT )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_STRING, STORAGE_UNIFORM )( TYPE_POINT )
        ( "ntransform", &ntransform_mv, TYPE_POINT )
            ( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_POINT )
        ( "ntransform", &ntransform_smv, TYPE_POINT )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_POINT )
        ( "depth", &depth, TYPE_FLOAT )
            ( TYPE_POINT )
        ( "calculatenormal", &calculatenormal, TYPE_POINT )
            ( TYPE_POINT )
    ;
    
    add_symbols()
        ( "comp", &comp, TYPE_FLOAT )
            ( TYPE_COLOR )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "comp", &comp, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_COLOR, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "setcomp", &setcomp, TYPE_NULL )
            ( TYPE_COLOR )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT )
        ( "setcomp", &setcomp, TYPE_NULL, STORAGE_UNIFORM )
            ( TYPE_COLOR, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "mix", &mix_vec3, TYPE_COLOR, STORAGE_UNIFORM )
            ( TYPE_COLOR, STORAGE_UNIFORM )( TYPE_COLOR, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "mix", &mix_vec3, TYPE_COLOR, STORAGE_VARYING )
            ( TYPE_COLOR, STORAGE_VARYING )( TYPE_COLOR, STORAGE_VARYING )( TYPE_FLOAT, STORAGE_VARYING )
        ( "ctransform", &ctransform_function, TYPE_COLOR )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_COLOR )
        ( "ctransform", &ctransform_function, TYPE_COLOR, STORAGE_UNIFORM )
            ( TYPE_STRING, STORAGE_UNIFORM )( TYPE_COLOR, STORAGE_UNIFORM )
    ;
    
    add_symbols()
        ( "comp", &comp_matrix, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "setcomp", &setcomp_matrix, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "determinant", &determinant, TYPE_FLOAT, STORAGE_UNIFORM )
            ( TYPE_MATRIX, STORAGE_UNIFORM )
        ( "translate", &translate_matrix, TYPE_MATRIX, STORAGE_UNIFORM )
            ( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_VECTOR, STORAGE_UNIFORM )
        ( "rotate", &rotate_matrix, TYPE_MATRIX, STORAGE_UNIFORM )
            ( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_FLOAT, STORAGE_UNIFORM )( TYPE_VECTOR, STORAGE_UNIFORM )
        ( "scale", &scale_matrix, TYPE_MATRIX, STORAGE_UNIFORM )
            ( TYPE_MATRIX, STORAGE_UNIFORM )( TYPE_POINT, STORAGE_UNIFORM )
    ;
    
    add_symbols()
        ( "ambient", &ambient, TYPE_COLOR )
        ( "diffuse", &diffuse, TYPE_COLOR )
            ( TYPE_NORMAL )
        ( "specular", &specular, TYPE_COLOR )
            ( TYPE_NORMAL )( TYPE_VECTOR )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "specularbrdf", &specularbrdf, TYPE_COLOR )
            ( TYPE_VECTOR )( TYPE_VECTOR )( TYPE_VECTOR )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "phong", &phong, TYPE_COLOR )
            ( TYPE_NORMAL )( TYPE_VECTOR )( TYPE_FLOAT, STORAGE_UNIFORM )
        ( "trace", &trace, TYPE_COLOR )
            ( TYPE_POINT )( TYPE_POINT )
    ;

    return *this;
}

AddSymbolHelper& AddSymbolHelper::add_surface_symbols()
{
    return add_symbols( SEGMENT_GRID )
        ( "P", TYPE_POINT )
        ( "N", TYPE_NORMAL )
        ( "I", TYPE_VECTOR )
        ( "s", TYPE_FLOAT )
        ( "t", TYPE_FLOAT )
        ( "Ci", TYPE_COLOR )
        ( "Oi", TYPE_COLOR )
        ( "Cs", TYPE_COLOR )
        ( "Os", TYPE_COLOR )
    ;
}

AddSymbolHelper& AddSymbolHelper::add_light_symbols()
{
    return add_symbols( SEGMENT_GRID )
        ( "Ps", TYPE_POINT )
        ( "N", TYPE_NORMAL )
        ( "Cl", TYPE_COLOR )
        ( "Ol", TYPE_COLOR )
    ;
}

AddSymbolHelper& AddSymbolHelper::add_volume_symbols()
{
    return add_symbols( SEGMENT_GRID )
        ( "P", TYPE_POINT )
        ( "I", TYPE_VECTOR )
        ( "Ci", TYPE_COLOR )
        ( "Oi", TYPE_COLOR )
    ;
}

AddSymbolHelper& AddSymbolHelper::add_displacement_symbols()
{
    return add_symbols( SEGMENT_GRID )
        ( "P", TYPE_POINT )
        ( "N", TYPE_NORMAL)
        ( "I", TYPE_VECTOR )
        ( "s", TYPE_FLOAT )
        ( "t", TYPE_FLOAT )
    ;
}

AddSymbolHelper& AddSymbolHelper::add_imager_symbols()
{
    return add_symbols( SEGMENT_GRID )
        ( "P", TYPE_POINT )
        ( "Ci", TYPE_COLOR )
        ( "Oi", TYPE_COLOR )
        ( "alpha", TYPE_FLOAT )
    ;        
}

AddSymbolHelper& AddSymbolHelper::add_illuminance_symbols()
{
    return add_symbols( SEGMENT_LIGHT )
        ( "L", TYPE_VECTOR )
        ( "Cl", TYPE_COLOR )
        ( "Ol", TYPE_COLOR )
    ;        
}

AddSymbolHelper& AddSymbolHelper::add_lighting_symbols()
{
    return add_symbols( SEGMENT_LIGHT )
        ( "L", TYPE_VECTOR )
    ;        
}
