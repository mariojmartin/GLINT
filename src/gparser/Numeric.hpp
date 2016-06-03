/*
Copyright (c) 2016 Mario J. Martin-Burgos <dominonurbs$gmail.com>
This softaware is licensed under Apache 2.0 license
http://www.apache.org/licenses/LICENSE-2.0

*******************************************************************************/

#ifndef H_GNUMERIC_H
#define H_GNUMERIC_H

#include <math.h>

#include "common/check_malloc.h"

#include "gdata.h"
#include "data_wrap.hpp"

template< typename T > const int vartype();
template<> const int vartype < _bool_ >(){ return t_bool; };
template<> const int vartype < _byte_ >(){ return t_byte; };
template<> const int vartype < _int_ >(){ return t_int; };
template<> const int vartype < _l64_ >(){ return t_l64; };
template<> const int vartype < _float_ >(){ return t_float; };
template<> const int vartype < _double_ >(){ return t_double; };

typedef void( *f_get_as_boolean )(_bool_* value);

struct Numeric : gVariable
{
    union Pool
    {
        _bool_ vbool;
        _byte_ vbyte;
        _int_ vint;
        _l64_ vl64;
        _float_ vfloat;
        _double_ vdouble;
    }pool;

    Numeric()
    {
        memset( this, 0, sizeof( Numeric ) );
        pvalue = &pool;
    }
};

#define v_bool_ vbool
#define v_byte_ vbyte
#define v_int_ vint
#define v_l64_ vl64
#define v_float_ vfloat
#define v_double_ vdouble

const char* numeric_type_name( const int type )
{
    static const char* s_und = "undefined";
    static const char* s_bool = "bool";
    static const char* s_byte = "byte";
    static const char* s_int = "int";
    static const char* s_int64 = "int64";
    static const char* s_float = "float";
    static const char* s_double = "double";
    static const char* s_grrr = "(unsupported type)";

    switch (type){
    case t_undefined:
        return s_und;
    case t_bool:
        return s_bool;
    case t_byte:
        return s_byte;
    case t_int:
        return s_int;
    case t_l64:
        return s_int64;
    case t_float:
        return s_float;
    case t_double:
        return s_double;
    default:
        return s_grrr;
    }
}

inline void numeric_copy( Numeric* ans, const Numeric a )
{
    ans->pool = a.pool;
    ans->type = a.type;
}

template< typename T >
void inline numeric_set( Numeric* var, const T value ){
    T* p = (T*)var->pvalue;
    *p = value;
    var->type = vartype<T>();
}

template<>
void inline numeric_set( Numeric* var, const _bool_ value ){
    _bool_* p = (_bool_*)var->pvalue;
    *p = value != 0 ? 1 : 0;
    var->type = t_bool;
}

template< typename T >
int numeric_explicit_get( const Numeric* const num, T* value ){
    switch (num->type){
    case t_bool:
        *value = T( num->pool.vbool );
        return GPARSE_OK;
    case t_byte:
        *value = T( num->pool.vbyte );
        return GPARSE_OK;
    case t_int:
        *value = T( num->pool.vint );
        return GPARSE_OK;
    case t_l64:
        *value = T( num->pool.vl64 );
        return GPARSE_OK;
    case t_float:
        *value = T( num->pool.vfloat );
        return GPARSE_OK;
    case t_double:
        *value = T( num->pool.vdouble );
        return GPARSE_OK;
    default:
        return GPARSE_ERROR;
    }
}

template<>
int numeric_explicit_get( const Numeric* const num, _bool_* value ){
    switch (num->type){
    case t_bool:
        *value = num->pool.vbool != 0;
        return GPARSE_OK;
    case t_byte:
        *value = num->pool.vbyte != 0;
        return GPARSE_OK;
    case t_int:
        *value = num->pool.vint != 0;
        return GPARSE_OK;
    case t_l64:
        *value = num->pool.vl64 != 0;
        return GPARSE_OK;
    case t_float:
        *value = num->pool.vfloat != 0;
        return GPARSE_OK;
    case t_double:
        *value = num->pool.vdouble != 0;
        return GPARSE_OK;
    default:
        return GPARSE_ERROR;
    }
}

int numeric_explicit_cast( Numeric* num, const int type ){
    switch (type){
    case t_bool:{
        _bool_ value;
        if (numeric_explicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_byte:{
        _byte_ value;
        if (numeric_explicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_int:{
        _int_ value;
        if (numeric_explicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_l64:{
        _l64_ value;
        if (numeric_explicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_float:{
        _float_ value;
        if (numeric_explicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_double:{
        _double_ value;
        if (numeric_explicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    default:
        return GPARSE_ERROR;
    }
}

template< typename T >
int numeric_implicit_get( const Numeric* const num, T* value ){
    switch (num->type){
    case t_byte:
        *value = T( num->pool.vbyte );
        return GPARSE_OK;
    case t_int:
        *value = T( num->pool.vint );
        return GPARSE_OK;
    case t_l64:
        *value = T( num->pool.vl64 );
        return GPARSE_OK;
    case t_float:
        *value = T( num->pool.vfloat );
        return GPARSE_OK;
    case t_double:
        *value = T( num->pool.vdouble );
        return GPARSE_OK;
    default:
        return GPARSE_ERROR;
    }
}

template<>
int numeric_implicit_get( const Numeric* const num, _bool_* value ){
    switch (num->type){
    case t_bool:
        *value = num->pool.vbool != 0;
        return GPARSE_OK;
    default:
        return GPARSE_ERROR;
    }
}

int numeric_implicit_cast( Numeric* num, const int type ){
    switch (type){
    case t_bool:{
        _bool_ value;
        if (numeric_implicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_byte:{
        _byte_ value;
        if (numeric_implicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_int:{
        _int_ value;
        if (numeric_implicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_l64:{
        _l64_ value;
        if (numeric_implicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_float:{
        _float_ value;
        if (numeric_implicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    case t_double:{
        _double_ value;
        if (numeric_implicit_get( num, &value )) return GPARSE_ERROR;
        numeric_set( num, value );
        return GPARSE_OK;
    }
    default:
        return GPARSE_ERROR;
    }
}

/*******************/
/* Unary operators */
/*******************/
inline int numeric_plus( Numeric* num, Parser* parser )
{
    if (num->type == t_byte || num->type == t_int || num->type == t_l64
        || num->type == t_float || num->type == t_double){
        return GPARSE_OK;
    }
    else{
        parser_error( parser, "Expecting a numeric value" );
        return GPARSE_ERROR;
    }
}

#define OP_NEG(T) case t_##T:{num->pool.v##T = -num->pool.v##T; return GPARSE_OK; }
inline int numeric_neg( Numeric* num, Parser* parser )
{
    switch (num->type){
        OP_NEG( _byte_ )
        OP_NEG( _int_ )
        OP_NEG( _l64_ )
        OP_NEG( _float_ )
        OP_NEG( _double_ )
    default:{
            parser_error( parser, "Expecting a numeric value" );
            return GPARSE_ERROR;
        }
    }
}
#undef OP_NEG

inline int numeric_not( Numeric* num, Parser* parser )
{
    if (num->type == t_bool){
        num->pool.vbool = !num->pool.vbool;
        return GPARSE_OK;
    }
    else{
        parser_error( parser, "Expecting a boolean value" );
        return GPARSE_ERROR;
    }
}

#define OP_BITINV(T) case t_##T:{num->pool.v##T = ~num->pool.v##T; return GPARSE_OK; }
inline int numeric_bitinv( Numeric* num, Parser* parser )
{
    switch(num->type){
        OP_BITINV( _byte_ )
        OP_BITINV( _int_ )
        OP_BITINV( _l64_ )
    default:
        parser_error( parser, "Expecting an integer value" );
        return GPARSE_ERROR;
    }
}
#undef OP_BITINV

/***********************/
/* Aritmetic operators */
/***********************/
inline int casting_arithmetic
    ( Numeric* _restrict_ a, Numeric* _restrict_ b, Parser* parser )
{
    switch (a->type){
    case t_byte:
        switch (b->type){
        case t_int:
            numeric_set( a, _int_( a->pool.vbyte ) ); break;
        case t_l64:
            numeric_set( a, _l64_( a->pool.vbyte ) ); break;
        case t_float:
            numeric_set( a, _float_( a->pool.vbyte ) ); break;
        case t_double:
            numeric_set( a, _double_( a->pool.vbyte ) ); break;
        default:
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_int:
        switch (b->type){
        case t_byte:
            numeric_set( b, _int_( b->pool.vbyte ) ); break;
        case t_l64:
            numeric_set( a, _l64_( a->pool.vint ) ); break;
        case t_float:
            numeric_set( a, _float_( a->pool.vint ) ); break;
        case t_double:
            numeric_set( a, _double_( a->pool.vint ) ); break;
        default:
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_l64:
        switch (b->type){
        case t_byte:
            numeric_set( b, _l64_( b->pool.vbyte ) ); break;
        case t_int:
            numeric_set( b, _l64_( b->pool.vint ) ); break;
        case t_float:
            numeric_set( a, _float_( a->pool.vl64 ) ); break;
        case t_double:
            numeric_set( a, _double_( a->pool.vl64 ) ); break;
        default:
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_float:
        switch (b->type){
        case t_byte:
            numeric_set( b, _float_( b->pool.vbyte ) ); break;
        case t_int:
            numeric_set( b, _float_( b->pool.vint ) ); break;
        case t_l64:
            numeric_set( b, _float_( b->pool.vl64 ) ); break;
        case t_double:
            numeric_set( a, _double_( a->pool.vfloat ) ); break;
        default:
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_double:
        switch (b->type){
        case t_byte:
            numeric_set( b, _double_( b->pool.vbyte ) ); break;
        case t_int:
            numeric_set( b, _double_( b->pool.vint ) ); break;
        case t_l64:
            numeric_set( b, _double_( b->pool.vl64 ) ); break;
        case t_float:
            numeric_set( b, _double_( b->pool.vfloat ) ); break;
        default:
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    default:
        return GPARSE_ERROR;
    }
}

inline int numeric_add
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    /* Upcasting between numeric types */
    if (ans->type != right->type){
        int status = casting_arithmetic( ans, right, parser );
        if (status != GPARSE_OK){
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
    }

    switch (ans->type){
    case t_byte:{
        ans->pool.vbyte += right->pool.vbyte;
        return GPARSE_OK;
    }
    case t_int:{
        ans->pool.vint += right->pool.vint;
        return GPARSE_OK;
    }
    case t_l64:{
        ans->pool.vl64 += right->pool.vl64;
        return GPARSE_OK;
    }
    case t_float:{
        ans->pool.vfloat += right->pool.vfloat;
        return GPARSE_OK;
    }
    case t_double:{
        ans->pool.vdouble += right->pool.vdouble;
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Expecting numeric operands" );
        return GPARSE_ERROR;
    }
}

inline int numeric_sub
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    /* Upcasting between numeric types */
    if (ans->type != right->type){
        int status = casting_arithmetic( ans, right, parser );
        if (status != GPARSE_OK){
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
    }

    switch (ans->type){
    case t_byte:{
        ans->pool.vbyte -= right->pool.vbyte;
        return GPARSE_OK;
    }
    case t_int:{
        ans->pool.vint -= right->pool.vint;
        return GPARSE_OK;
    }
    case t_l64:{
        ans->pool.vl64 -= right->pool.vl64;
        return GPARSE_OK;
    }
    case t_float:{
        ans->pool.vfloat -= right->pool.vfloat;
        return GPARSE_OK;
    }
    case t_double:{
        ans->pool.vdouble -= right->pool.vdouble;
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Expecting numeric operands" );
        return GPARSE_ERROR;
    }
}

inline int numeric_mul
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    /* Upcasting between numeric types */
    if (ans->type != right->type){
        int status = casting_arithmetic( ans, right, parser );
        if (status != GPARSE_OK){
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
    }

    switch (ans->type){
    case t_byte:{
        ans->pool.vbyte *= right->pool.vbyte;
        return GPARSE_OK;
    }
    case t_int:{
        ans->pool.vint *= right->pool.vint;
        return GPARSE_OK;
    }
    case t_l64:{
        ans->pool.vl64 *= right->pool.vl64;
        return GPARSE_OK;
    }
    case t_float:{
        ans->pool.vfloat *= right->pool.vfloat;
        return GPARSE_OK;
    }
    case t_double:{
        ans->pool.vdouble *= right->pool.vdouble;
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Expecting numeric operands" );
        return GPARSE_ERROR;
    }
}

int numeric_div
    ( Numeric* _restrict_ a, Numeric* _restrict_ b, Parser* parser )
{
    /* Upcasting to floating point single or double, with preference to double */
    switch (a->type){
    case t_byte:
        switch (b->type){
        case t_byte:{
            _double_ v = _double_( a->pool.vbyte ) / _double_( b->pool.vbyte );
            numeric_set( a, v ); break;
        }
        case t_int:{
            _double_ v = _double_( a->pool.vbyte ) / _double_( b->pool.vint );
            numeric_set( a, v ); break;
        }
        case t_l64:{
            _double_ v = _double_( a->pool.vbyte ) / _double_( b->pool.vl64 );
            numeric_set( a, v ); break;
        }
        case t_float:{
            _float_ v = _float_( a->pool.vbyte ) / b->pool.vfloat;
            numeric_set( a, v ); break;
        }
        case t_double:{
            _double_ v = _double_( a->pool.vbyte ) / b->pool.vdouble;
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_int:
        switch (b->type){
        case t_byte:{
            _double_ v = _double_( a->pool.vint ) / _double_( b->pool.vbyte );
            numeric_set( a, v ); break;
        }
        case t_int:{
            _double_ v = _double_( a->pool.vint ) / _double_( b->pool.vint );
            numeric_set( a, v ); break;
        }
        case t_l64:{
            _double_ v = _double_( a->pool.vint ) / _double_( b->pool.vl64 );
            numeric_set( a, v ); break;
        }
        case t_float:{
            _float_ v = _float_( a->pool.vint ) / b->pool.vfloat;
            numeric_set( a, v ); break;
        }
        case t_double:{
            _double_ v = _double_( a->pool.vint ) / b->pool.vdouble;
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_l64:
        switch (b->type){
        case t_byte:{
            _double_ v = _double_( a->pool.vl64 ) / _double_( b->pool.vbyte );
            numeric_set( a, v ); break;
            break;
        }
        case t_int:{
            _double_ v = _double_( a->pool.vl64 ) / _double_( b->pool.vint );
            numeric_set( a, v ); break;
            break;
        }
        case t_l64:{
            _double_ v = _double_( a->pool.vl64 ) / _double_( b->pool.vl64 );
            numeric_set( a, v ); break;
            break;
        }
        case t_float:{
            _float_ v = _float_( a->pool.vl64 ) / b->pool.vfloat;
            numeric_set( a, v ); break;
        }
        case t_double:{
            _double_ v = _double_( a->pool.vl64 ) / b->pool.vdouble;
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_float:
        switch (b->type){
        case t_byte:
            a->pool.vfloat /= _float_( b->pool.vbyte ); break;
        case t_int:
            a->pool.vfloat /= _float_( b->pool.vint ); break;
        case t_l64:
            a->pool.vfloat /= _float_( b->pool.vl64 ); break;
        case t_float:
            a->pool.vfloat /= b->pool.vfloat; break;
        case t_double:{
            _double_ v = _double_( a->pool.vfloat ) / b->pool.vdouble;
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_double:
        switch (b->type){
        case t_byte:
            a->pool.vdouble /= _double_( b->pool.vbyte ); break;
        case t_int:
            a->pool.vdouble /= _double_( b->pool.vint ); break;
        case t_l64:
            a->pool.vdouble /= _double_( b->pool.vl64 ); break;
        case t_float:
            a->pool.vdouble /= _double_( b->pool.vfloat ); break;
        case t_double:
            a->pool.vdouble /= b->pool.vdouble; break;
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    default:
        parser_error( parser, "Expecting numeric operands" );
        return GPARSE_ERROR;
    }
}

int numeric_intdiv
    ( Numeric* _restrict_ a, Numeric* _restrict_ b, Parser* parser )
{
    /* Downcasting to integer */
    switch (a->type){
    case t_byte:
        switch (b->type){
        case t_byte:
            a->pool.vbyte /= b->pool.vbyte; break;
        case t_int:
            a->pool.vbyte /= b->pool.vint; break;
        case t_l64:{
            int v = int( _l64_( a->pool.vbyte ) / b->pool.vl64 );
            a->pool.vbyte = v;
        }
        case t_float:
            a->pool.vbyte /= _int_( b->pool.vfloat ); break;
        case t_double:
            a->pool.vbyte /= _int_( b->pool.vdouble ); break;
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_int:
        switch (b->type){
        case t_byte:
            a->pool.vint /= b->pool.vbyte; break;
        case t_int:
            a->pool.vint /= b->pool.vint; break;
        case t_l64:{
            _int_ v = _int_( _l64_( a->pool.vint ) / b->pool.vl64 );
            a->pool.vint = v;
        }
        case t_float:
            a->pool.vint /= _int_( b->pool.vfloat ); break;
        case t_double:
            a->pool.vint /= _int_( b->pool.vdouble ); break;
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_l64:
        switch (b->type){
        case t_byte:
            a->pool.vl64 /= b->pool.vbyte; break;
        case t_int:
            a->pool.vl64 /= b->pool.vint; break;
        case t_l64:
            a->pool.vl64 /= b->pool.vl64; break;
        case t_float:
            a->pool.vl64 /= _l64_( b->pool.vfloat ); break;
        case t_double:
            a->pool.vl64 /= _l64_( b->pool.vdouble ); break;
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_float:
        switch (b->type){
        case t_byte:{
            _int_ v = _int_( a->pool.vfloat ) / b->pool.vbyte;
            numeric_set( a, v ); break;
        }
        case t_int:{
            _int_ v = _int_( a->pool.vfloat ) / b->pool.vint;
            numeric_set( a, v ); break;
        }
        case t_l64:{
            _int_ v = _int_( _l64_( a->pool.vfloat ) / b->pool.vl64 );
            numeric_set( a, v ); break;
        }
        case t_float:{
            _int_ v = _int_( a->pool.vfloat ) / _int_( b->pool.vfloat );
            numeric_set( a, v ); break;
        }
        case t_double:{
            _int_ v = _int_( a->pool.vfloat ) / _int_( b->pool.vdouble );
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_double:
        switch (b->type){
        case t_byte:{
            _int_ v = _int_( a->pool.vdouble ) / b->pool.vbyte;
            numeric_set( a, v ); break;
        }
        case t_int:{
            _int_ v = _int_( a->pool.vdouble ) / b->pool.vint;
            numeric_set( a, v ); break;
        }
        case t_l64:{
            _int_ v = _int_( _l64_( a->pool.vdouble ) / b->pool.vl64 );
            numeric_set( a, v ); break;
        }
        case t_float:{
            _int_ v = _int_( a->pool.vdouble ) / _int_( b->pool.vfloat );
            numeric_set( a, v ); break;
        }
        case t_double:{
            _int_ v = _int_( a->pool.vdouble ) / _int_( b->pool.vdouble );
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    default:
        parser_error( parser, "Expecting numeric operands" );
        return GPARSE_ERROR;
    }
}

inline int numeric_remainder
( Numeric* _restrict_ a, Numeric* _restrict_ b, Parser* parser )
{
    /* Downcasting to integer */
    switch (a->type){
    case t_byte:
        switch (b->type){
        case t_byte:
            a->pool.vbyte %= b->pool.vbyte; break;
        case t_int:
            a->pool.vbyte %= b->pool.vint; break;
        case t_l64:
            a->pool.vbyte %= b->pool.vl64; break;
        case t_float:
            a->pool.vbyte %= int( b->pool.vfloat ); break;
        case t_double:
            a->pool.vbyte %= int( b->pool.vdouble ); break;
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_int:
        switch (b->type){
        case t_byte:
            a->pool.vint %= b->pool.vbyte; break;
        case t_int:
            a->pool.vint %= b->pool.vint; break;
        case t_l64:
            a->pool.vint %= b->pool.vl64; break;
        case t_float:
            a->pool.vint %= int( b->pool.vfloat ); break;
        case t_double:
            a->pool.vint %= int( b->pool.vdouble ); break;
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_l64:
        switch (b->type){
        case t_byte:
            a->pool.vl64 %= b->pool.vbyte; break;
        case t_int:
            a->pool.vl64 %= b->pool.vint; break;
        case t_l64:
            a->pool.vl64 %= b->pool.vl64; break;
        case t_float:
            a->pool.vl64 %= _l64_( b->pool.vfloat ); break;
        case t_double:
            a->pool.vl64 %= _l64_( b->pool.vdouble ); break;
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_float:
        switch (b->type){
        case t_byte:{
            _int_ v = _int_( a->pool.vfloat ) % b->pool.vbyte;
            numeric_set( a, v ); break;
        }
        case t_int:{
            _int_ v = _int_( a->pool.vfloat ) % b->pool.vint;
            numeric_set( a, v ); break;
        }
        case t_l64:{
            _int_ v = _int_( a->pool.vfloat ) % b->pool.vl64;
            numeric_set( a, v ); break;
        }
        case t_float:{
            _int_ v = _int_( a->pool.vfloat ) % _int_( b->pool.vfloat );
            numeric_set( a, v ); break;
        }
        case t_double:{
            _int_ v = _int_( a->pool.vfloat ) % _int_( b->pool.vdouble );
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_double:
        switch (b->type){
        case t_byte:{
            _int_ v = _int_( a->pool.vdouble ) % b->pool.vbyte;
            numeric_set( a, v ); break;
        }
        case t_int:{
            _int_ v = _int_( a->pool.vdouble ) % b->pool.vint;
            numeric_set( a, v ); break;
        }
        case t_l64:{
            _int_ v = _int_( a->pool.vdouble ) % b->pool.vl64;
            numeric_set( a, v ); break;
        }
        case t_float:{
            _int_ v = _int_( a->pool.vdouble ) % _int_( b->pool.vfloat );
            numeric_set( a, v ); break;
        }
        case t_double:{
            _int_ v = _int_( a->pool.vdouble ) % _int_( b->pool.vdouble );
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    default:
        parser_error( parser, "Expecting numeric operands" );
        return GPARSE_ERROR;
    }
}

int numeric_pow
    ( Numeric* _restrict_ a, Numeric* _restrict_ b, Parser* parser )
{
    /* Upcasting to floating point single or double, with preference to double */
    switch (a->type){
    case t_byte:
        switch (b->type){
        case t_byte:{
            _double_ v = pow( double( a->pool.vbyte ), double( b->pool.vbyte ) );
            numeric_set( a, v ); break;
        }
        case t_int:{
            _double_ v = pow( double( a->pool.vbyte ), double( b->pool.vint ) );
            numeric_set( a, v ); break;
        }
        case t_l64:{
            _double_ v = pow( double( a->pool.vbyte ), double( b->pool.vl64 ) );
            numeric_set( a, v ); break;
        }
        case t_float:{
            _float_ v = pow( float( a->pool.vbyte ), b->pool.vfloat );
            numeric_set( a, v ); break;
        }
        case t_double:{
            _double_ v = pow( double( a->pool.vbyte ), b->pool.vdouble );
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_int:
        switch (b->type){
        case t_byte:{
            double v = pow( double( a->pool.vbyte ), double( b->pool.vbyte ) );
            numeric_set( a, v ); break;
        }
        case t_int:{
            double v = pow( double( a->pool.vint ), double( b->pool.vint ) );
            numeric_set( a, v ); break;
        }
        case t_l64:{
            double v = pow( double( a->pool.vint ), double( b->pool.vl64 ) );
            numeric_set( a, v ); break;
        }
        case t_float:{
            float v = pow( float( a->pool.vint ), b->pool.vfloat );
            numeric_set( a, v ); break;
        }
        case t_double:{
            double v = pow( double( a->pool.vint ), b->pool.vdouble );
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_l64:
        switch (b->type){
        case t_byte:{
            double v = pow( double( a->pool.vl64 ), double( b->pool.vbyte ) );
            numeric_set( a, v ); break;
        }
        case t_int:{
            double v = pow( double( a->pool.vl64 ), double( b->pool.vint ) );
            numeric_set( a, v ); break;
        }
        case t_l64:{
            double v = pow( double( a->pool.vl64 ), double( b->pool.vl64 ) );
            numeric_set( a, v ); break;
        }
        case t_float:{
            float v = pow( float( a->pool.vl64 ), b->pool.vfloat );
            numeric_set( a, v ); break;
        }
        case t_double:{
            double v = pow( double( a->pool.vl64 ), b->pool.vdouble );
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_float:
        switch (b->type){
        case t_byte:
            a->pool.vfloat = pow( a->pool.vfloat, float( b->pool.vbyte ) ); break;
        case t_int:
            a->pool.vfloat = pow( a->pool.vfloat, float( b->pool.vint ) ); break;
        case t_l64:
            a->pool.vfloat = pow( a->pool.vfloat, float( b->pool.vl64 ) ); break;
        case t_float:
            a->pool.vfloat = pow( a->pool.vfloat, b->pool.vfloat ); break;
        case t_double:{
            double v = double( a->pool.vfloat ) / b->pool.vdouble;
            numeric_set( a, v ); break;
        }
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case t_double:
        switch (b->type){
        case t_byte:
            a->pool.vdouble = pow( a->pool.vdouble, double( b->pool.vbyte ) ); break;
        case t_int:
            a->pool.vdouble = pow( a->pool.vdouble, double( b->pool.vint ) ); break;
        case t_l64:
            a->pool.vdouble = pow( a->pool.vdouble, double( b->pool.vl64 ) ); break;
        case t_float:
            a->pool.vdouble = pow( a->pool.vdouble, double( b->pool.vfloat ) ); break;
        case t_double:
            a->pool.vdouble = pow( a->pool.vdouble, double( b->pool.vdouble ) ); break;
        default:
            parser_error( parser, "Expecting numeric operands" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    default:
        parser_error( parser, "Expecting numeric operands" );
        return GPARSE_ERROR;
    }
}

/*********************/
/* Boolean operators */
/*********************/
inline int numeric_equal
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    /* Check types and cast if possible 
     * How can be compared 3.1 == 3.1f 
     * Actually in C, the above is false. */
    int lt = ans->type;
    int rt = right->type;

    /* Check types and cast, so types are the same. */
    if (lt != rt){
        int status = casting_arithmetic( ans, right, parser );
        if (status != GPARSE_OK){
            parser_error( parser, "Operation not defined between types (%s) (%s)"
                , numeric_type_name( lt ), numeric_type_name( rt ) );
            return status;
        }
    }

    switch (ans->type){
    case t_bool:{
        ans->pool.vbool = _bool_(ans->pool.vbool == right->pool.vbool);
        return GPARSE_OK;
    }
    case t_byte:{
        numeric_set( ans, _bool_( ans->pool.vbyte == right->pool.vbyte ) );
        return GPARSE_OK;
    }
    case t_int:{
        numeric_set( ans, _bool_( ans->pool.vint == right->pool.vint ) );
        return GPARSE_OK;
    }
    case t_l64:{
        numeric_set( ans, _bool_( ans->pool.vl64 == right->pool.vl64 ) );
        return GPARSE_OK;
    }
    case t_float:
        parser_error( parser, 
            "a==b between floating point numbers is not a valid operation."
            " Consider using \"abs(a-b) < eps\"." );
        return GPARSE_ERROR;
    case t_double:
        parser_error( parser,
            "a==b between floating point numbers is not a valid operation."
            " Consider using \"abs(a-b) < eps\"." );
        return GPARSE_ERROR;

    default:
        parser_error( parser, "Operation not defined between types (%s) (%s)"
            , numeric_type_name( lt ), numeric_type_name( rt ) );
        return GPARSE_ERROR;
    }
}

inline int numeric_notequal
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    int lt = ans->type;
    int rt = right->type;

    /* Check types and cast, so types are the same. */
    if (lt != rt){
        int status = casting_arithmetic( ans, right, parser );
        if (status != GPARSE_OK){
            parser_error( parser, "Operation not defined between types (%s) (%s)"
                , numeric_type_name( lt ), numeric_type_name( rt ) );
            return status;
        }
    }

    switch (ans->type){
    case t_bool:{
        ans->pool.vbool = _bool_(ans->pool.vbool != right->pool.vbool);
        return GPARSE_OK;
    }
    case t_byte:{
        numeric_set( ans, _bool_( ans->pool.vbyte != right->pool.vbyte ) );
        return GPARSE_OK;
    }
    case t_int:{
        numeric_set( ans, _bool_( ans->pool.vint != right->pool.vint ) );
        return GPARSE_OK;
    }
    case t_l64:{
        numeric_set( ans, _bool_( ans->pool.vl64 != right->pool.vl64 ) );
        return GPARSE_OK;
    }
    case t_float:
        parser_error( parser,
            "a==b between floating point numbers is not a valid operation."
            " Consider using \"abs(a-b) > eps\"." );
        return GPARSE_ERROR;
    case t_double:
        parser_error( parser,
            "a==b between floating point numbers is not a valid operation."
            " Consider using \"abs(a-b) > eps\"." );
        return GPARSE_ERROR;
    default:
        parser_error( parser, "Operation not defined between types (%s) (%s)"
            , numeric_type_name( lt ), numeric_type_name( rt ) );
        return GPARSE_ERROR;
    }
}

inline int numeric_greater
( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    int lt = ans->type;
    int rt = right->type;

    /* Check types and cast, so types are the same. */
    if (lt != rt){
        int status = casting_arithmetic( ans, right, parser );
        if (status != GPARSE_OK){
            parser_error( parser, "Operation not defined between types (%s) (%s)"
                , numeric_type_name( lt ), numeric_type_name( rt ) );
            return status;
        }
    }

    switch (ans->type){
    case t_byte:{
        numeric_set( ans, _bool_( ans->pool.vbyte > right->pool.vbyte ) );
        return GPARSE_OK;
    }
    case t_int:{
        numeric_set( ans, _bool_( ans->pool.vint > right->pool.vint ) );
        return GPARSE_OK;
    }
    case t_l64:{
        numeric_set( ans, _bool_( ans->pool.vl64 > right->pool.vl64 ) );
        return GPARSE_OK;
    }
    case t_float:{
        numeric_set( ans, _bool_( ans->pool.vfloat > right->pool.vfloat ) );
        return GPARSE_OK;
    }
    case t_double:{
        numeric_set( ans, _bool_( ans->pool.vdouble > right->pool.vdouble ) );
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Operation not defined between types (%s) (%s)"
            , numeric_type_name( lt ), numeric_type_name( rt ) );
        return GPARSE_ERROR;
    }
}

inline int numeric_greater_equal
( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    int lt = ans->type;
    int rt = right->type;

    /* Check types and cast, so types are the same. */
    if (lt != rt){
        int status = casting_arithmetic( ans, right, parser );
        if (status != GPARSE_OK){
            parser_error( parser, "Operation not defined between types (%s) (%s)"
                , numeric_type_name( lt ), numeric_type_name( rt ) );
            return status;
        }
    }

    switch (ans->type){
    case t_byte:{
        numeric_set( ans, _bool_( ans->pool.vbyte >= right->pool.vbyte ) );
        return GPARSE_OK;
    }
    case t_int:{
        numeric_set( ans, _bool_( ans->pool.vint >= right->pool.vint ) );
        return GPARSE_OK;
    }
    case t_l64:{
        numeric_set( ans, _bool_( ans->pool.vl64 >= right->pool.vl64 ) );
        return GPARSE_OK;
    }
    case t_float:{
        numeric_set( ans, _bool_( ans->pool.vfloat >= right->pool.vfloat ) );
        return GPARSE_OK;
    }
    case t_double:{
        numeric_set( ans, _bool_( ans->pool.vdouble >= right->pool.vdouble ) );
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Operation not defined between types (%s) (%s)"
            , numeric_type_name( lt ), numeric_type_name( rt ) );
        return GPARSE_ERROR;
    }
}

inline int numeric_less
( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    int lt = ans->type;
    int rt = right->type;

    /* Check types and cast, so types are the same. */
    if (lt != rt){
        int status = casting_arithmetic( ans, right, parser );
        if (status != GPARSE_OK){
            parser_error( parser, "Operation not defined between types (%s) (%s)"
                , numeric_type_name( lt ), numeric_type_name( rt ) );
            return status;
        }
    }

    switch (ans->type){
    case t_byte:{
        numeric_set( ans, _bool_( ans->pool.vbyte < right->pool.vbyte ) );
        return GPARSE_OK;
    }
    case t_int:{
        numeric_set( ans, _bool_( ans->pool.vint < right->pool.vint ) );
        return GPARSE_OK;
    }
    case t_l64:{
        numeric_set( ans, _bool_( ans->pool.vl64 < right->pool.vl64 ) );
        return GPARSE_OK;
    }
    case t_float:{
        numeric_set( ans, _bool_( ans->pool.vfloat < right->pool.vfloat ) );
        return GPARSE_OK;
    }
    case t_double:{
        numeric_set( ans, _bool_( ans->pool.vdouble < right->pool.vdouble ) );
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Operation not defined between types (%s) (%s)"
            , numeric_type_name( lt ), numeric_type_name( rt ) );
        return GPARSE_ERROR;
    }
}

inline int numeric_less_equal
( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    int lt = ans->type;
    int rt = right->type;

    /* Check types and cast, so types are the same. */
    if (lt != rt){
        int status = casting_arithmetic( ans, right, parser );
        if (status != GPARSE_OK){
            parser_error( parser, "Operation not defined between types (%s) (%s)"
                , numeric_type_name( lt ), numeric_type_name( rt ) );
            return status;
        }
    }

    switch (ans->type){
    case t_byte:{
        numeric_set( ans, _bool_( ans->pool.vbyte <= right->pool.vbyte ) );
        return GPARSE_OK;
    }
    case t_int:{
        numeric_set( ans, _bool_( ans->pool.vint <= right->pool.vint ) );
        return GPARSE_OK;
    }
    case t_l64:{
        numeric_set( ans, _bool_( ans->pool.vl64 <= right->pool.vl64 ) );
        return GPARSE_OK;
    }
    case t_float:{
        numeric_set( ans, _bool_( ans->pool.vfloat <= right->pool.vfloat ) );
        return GPARSE_OK;
    }
    case t_double:{
        numeric_set( ans, _bool_( ans->pool.vdouble <= right->pool.vdouble ) );
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Operation not defined between types (%s) (%s)"
            , numeric_type_name( lt ), numeric_type_name( rt ) );
        return GPARSE_ERROR;
    }
}

inline int numeric_and
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    if (ans->type != t_bool || right->type != t_bool){
        parser_error( parser, "Expecting boolean types" );
        return GPARSE_ERROR;
    }

    ans->pool.vbool = _bool_(ans->pool.vbool && right->pool.vbool);
    return GPARSE_OK;
}

inline int numeric_or
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    if (ans->type != t_bool || right->type != t_bool){
        parser_error( parser, "Expecting boolean types" );
        return GPARSE_ERROR;
    }

    ans->pool.vbool = _bool_( ans->pool.vbool || right->pool.vbool );
    return GPARSE_OK;
}

inline int numeric_bit_and
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    switch (ans->type){
    case t_byte:{
        switch (right->type){
        case t_byte:
            ans->pool.vbyte &= right->pool.vbyte; break;
        case t_int:{
            _int_ v = _int_( ans->pool.vbyte ) & right->pool.vint;
            numeric_set( ans, v ); break;
        }
        case t_l64:{
            _l64_ v = _l64_( ans->pool.vint ) & right->pool.vl64;
            numeric_set( ans, v ); break;
        }
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_int:{
        switch (right->type){
        case t_byte:
            ans->pool.vint &= _int_( right->pool.vbyte ); break;
        case t_int:
            ans->pool.vint &= right->pool.vint; break;
        case t_l64:{
            _l64_ v = _l64_( ans->pool.vint ) & right->pool.vl64;
            numeric_set( ans, v ); break;
        }
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_l64:{
        switch (right->type){
        case t_byte:
            ans->pool.vl64 &= _l64_( right->pool.vbyte ); break;
        case t_int:
            ans->pool.vl64 &= _l64_( right->pool.vint ); break;
        case t_l64:{
            ans->pool.vl64 &= right->pool.vl64; break;
        }
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Expecting integer types" );
        return GPARSE_ERROR;
    }

    return GPARSE_OK;
}

inline int numeric_bit_xor
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    switch (ans->type){
    case t_byte:{
        switch (right->type){
        case t_byte:
            ans->pool.vbyte ^= right->pool.vbyte; break;
        case t_int:{
            _int_ v = _int_( ans->pool.vbyte ) ^ right->pool.vint;
            numeric_set( ans, v ); break;
        }
        case t_l64:{
            _l64_ v = _l64_( ans->pool.vint ) ^ right->pool.vl64;
            numeric_set( ans, v ); break;
        }
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_int:{
        switch (right->type){
        case t_byte:
            ans->pool.vint ^= _int_( right->pool.vbyte ); break;
        case t_int:
            ans->pool.vint ^= right->pool.vint; break;
        case t_l64:{
            _l64_ v = _l64_( ans->pool.vint ) ^ right->pool.vl64;
            numeric_set( ans, v ); break;
        }
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_l64:{
        switch (right->type){
        case t_byte:
            ans->pool.vl64 ^= _l64_( right->pool.vbyte ); break;
        case t_int:
            ans->pool.vl64 ^= _l64_( right->pool.vint ); break;
        case t_l64:{
            ans->pool.vl64 ^= right->pool.vl64; break;
        }
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Expecting integer types" );
        return GPARSE_ERROR;
    }

    return GPARSE_OK;
}

inline int numeric_bit_or
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    switch (ans->type){
    case t_byte:{
        switch (right->type){
        case t_byte:
            ans->pool.vbyte |= right->pool.vbyte; break;
        case t_int:{
            _int_ v = _int_( ans->pool.vbyte ) | right->pool.vint;
            numeric_set( ans, v ); break;
        }
        case t_l64:{
            _l64_ v = _l64_( ans->pool.vint ) | right->pool.vl64;
            numeric_set( ans, v ); break;
        }
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_int:{
        switch (right->type){
        case t_byte:
            ans->pool.vint |= int( right->pool.vbyte ); break;
        case t_int:
            ans->pool.vint |= right->pool.vint; break;
        case t_l64:{
            _l64_ v = _l64_( ans->pool.vint ) | right->pool.vl64;
            numeric_set( ans, v ); break;
        }
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_l64:{
        switch (right->type){
        case t_byte:
            ans->pool.vl64 |= _l64_( right->pool.vbyte ); break;
        case t_int:
            ans->pool.vl64 |= _l64_( right->pool.vint ); break;
        case t_l64:{
            ans->pool.vl64 |= right->pool.vl64; break;
        }
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Expecting integer types" );
        return GPARSE_ERROR;
    }

    return GPARSE_OK;
}

inline int numeric_lshift
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    switch (ans->type){
    case t_byte:{
        switch (right->type){
        case t_byte:
            ans->pool.vbyte <<= right->pool.vbyte; break;
        case t_int:
            ans->pool.vbyte <<= right->pool.vint; break;
        case t_l64:
            ans->pool.vbyte <<= int( right->pool.vl64 ); break;
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_int:{
        switch (right->type){
        case t_byte:
            ans->pool.vint <<= right->pool.vbyte; break;
        case t_int:
            ans->pool.vint <<= right->pool.vint; break;
        case t_l64:
            ans->pool.vint <<= int( right->pool.vl64 ); break;
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_l64:{
        switch (right->type){
        case t_byte:
            ans->pool.vl64 <<= right->pool.vbyte; break;
        case t_int:
            ans->pool.vl64 <<= right->pool.vint; break;
        case t_l64:
            ans->pool.vl64 <<= int( right->pool.vl64 ); break;
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Expecting integer types" );
        return GPARSE_ERROR;
    }

    return GPARSE_OK;
}

inline int numeric_rshift
    ( Numeric* _restrict_ ans, Numeric* _restrict_ right, Parser* parser )
{
    switch (ans->type){
    case t_byte:{
        switch (right->type){
        case t_byte:
            ans->pool.vbyte >>= right->pool.vbyte; break;
        case t_int:
            ans->pool.vbyte >>= right->pool.vint; break;
        case t_l64:
            ans->pool.vbyte >>= int( right->pool.vl64 ); break;
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_int:{
        switch (right->type){
        case t_byte:
            ans->pool.vint >>= right->pool.vbyte; break;
        case t_int:
            ans->pool.vint >>= right->pool.vint; break;
        case t_l64:
            ans->pool.vint >>= int( right->pool.vl64 ); break;
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    case t_l64:{
        switch (right->type){
        case t_byte:
            ans->pool.vl64 >>= right->pool.vbyte; break;
        case t_int:
            ans->pool.vl64 >>= right->pool.vint; break;
        case t_l64:
            ans->pool.vl64 >>= int( right->pool.vl64 ); break;
        default:
            parser_error( parser, "Expecting integer types" );
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    }
    default:
        parser_error( parser, "Expecting integer types" );
        return GPARSE_ERROR;
    }

    return GPARSE_OK;
}

#endif /* H_GNUMERIC_H */
