/***
Author: Mario J. Martin <dominonurbs$gmail.com>

Variable types look for simplicity but also compatibility with C.


bool: Boolean type that only admits true and false
byte: equivalent to an integer of 8 bits
int: Equivalent to C's int. The size depends of the machine
int64: Integer of 64 bits
float: Same as C, this type is mostly for compatibility
double: Default for arithmetic operations. 
*******************************************************************************/

#include "data_wrap.hpp"

#ifndef H_GVARIABLE_H
#define H_GVARIABLE_H

/* Creates the variable */
int variable_alloc( gVariable* var, const int type )
{
    switch (type){
    case t_bool:{
        /* Create the variable */
        var->size = sizeof( _bool_ );
        var->pvalue = (_bool_*)malloc( var->size );
        var->type = t_bool;
        return GPARSE_OK;
    }

    case t_byte:{
        /* Create the variable */
        var->size = sizeof( _byte_ );
        var->pvalue = (_byte_*)malloc( var->size );
        var->type = t_byte;
        return GPARSE_OK;
    }

    case t_int:{
        /* Create the variable */
        var->size = sizeof( _int_ );
        var->pvalue = (_int_*)malloc( var->size );
        var->type = t_int;
        return GPARSE_OK;
    }

    case t_l64:{
        /* Create the variable */
        var->size = sizeof( _l64_ );
        var->pvalue = (_l64_*)malloc( var->size );
        var->type = t_l64;
        return GPARSE_OK;
    }

    case t_float:{
        /* Create the variable */
        var->size = sizeof( _float_ );
        var->pvalue = (_float_*)malloc( var->size );
        var->type = t_float;
        return GPARSE_OK;
    }

    case t_double:{
        /* Create the variable */
        var->size = sizeof( _double_ );
        var->pvalue = (_double_*)malloc( var->size );
        var->type = t_double;
        return GPARSE_OK;
    }

    default:
        var->type = t_undefined;
        return GPARSE_ERROR;
    }
}

/* Assign the variable when the types are the same 'var->type == num->type' */
int variable_assign( gVariable* var, Numeric* num )
{
    if (var->type == num->type){
        memcpy( var->pvalue, num->pvalue, var->size );
    }
    else{
        if (numeric_implicit_cast( num, var->type )) {
            return GPARSE_ERROR;
        }
        memcpy( var->pvalue, num->pvalue, var->size );
    }
    return GPARSE_OK;
}

/* Assign the variable, changing the variable type to the value */
void variable_dynamic_assign( gVariable* var, const Numeric* num )
{
    /* Always assign the left value, without checking the type */
    if (var->type != num->type){
        free( var->pvalue );
        var->pvalue = nullptr;
        variable_alloc( var, num->type );
    }
    memcpy( var->pvalue, num->pvalue, var->size );
}

template< typename T >
int variable_get_as( const gVariable* const var, T* value ){
    switch (var->type){
    case t_bool:{
        _bool_* pvalue = (_bool_*)var->pvalue;
        *value = (T)*pvalue;
        return GPARSE_OK;
    }
    case t_byte:{
        _byte_* pvalue = (_byte_*)var->pvalue;
        *value = (T)*pvalue;
        return GPARSE_OK;
    }
    case t_int:{
        _int_* pvalue = (_int_*)var->pvalue;
        *value = (T)*pvalue;
        return GPARSE_OK;
    }
    case t_l64:{
        _l64_* pvalue = (_l64_*)var->pvalue;
        *value = (T)*pvalue;
        return GPARSE_OK;
    }
    case t_float:{
        _float_* pvalue = (_float_*)var->pvalue;
        *value = (T)*pvalue;
        return GPARSE_OK;
    }
    case t_double:{
        _double_* pvalue = (_double_*)var->pvalue;
        *value = (T)*pvalue;
        return GPARSE_OK;
    }
    default:
        return GPARSE_ERROR;
    }
}

int variable_get_as( const gVariable* const var, _bool_* value ){
    switch (var->type){
    case t_bool:{
        _bool_* pvalue = (_bool_*)var->pvalue;
        *value = *pvalue;
        return GPARSE_OK;
    }
    case t_byte:{
        _byte_* pvalue = (_byte_*)var->pvalue;
        *value = *pvalue != 0 ? 1 : 0;
        return GPARSE_OK;
    }
    case t_int:{
        _int_* pvalue = (_int_*)var->pvalue;
        *value = *pvalue != 0 ? 1 : 0;
        return GPARSE_OK;
    }
    case t_l64:{
        _l64_* pvalue = (_l64_*)var->pvalue;
        *value = *pvalue != 0 ? 1 : 0;
        return GPARSE_OK;
    }
    case t_float:{
        _float_* pvalue = (_float_*)var->pvalue;
        *value = *pvalue != 0 ? 1 : 0;
        return GPARSE_OK;
    }
    case t_double:{
        _double_* pvalue = (_double_*)var->pvalue;
        *value = *pvalue != 0 ? 1 : 0;
        return GPARSE_OK;
    }
    default:
        return GPARSE_ERROR;
    }
}

/* Assign the value of a variable */
int variable_assign( gVariable* _restrict_ a, const gVariable* _restrict_ const b )
{
    if (a->type == b->type){
        switch (a->type){
        case t_bool:{
            _bool_* pa = (_bool_*)a->pvalue;
            _bool_* pb = (_bool_*)b->pvalue;
            *pa = *pb;
            return GPARSE_OK;
        }

        case t_byte:{
            _byte_* pa = (_byte_*)a->pvalue;
            _byte_* pb = (_byte_*)b->pvalue;
            *pa = *pb;
            return GPARSE_OK;
        }

        case t_int:{
            _int_* pa = (_int_*)a->pvalue;
            _int_* pb = (_int_*)b->pvalue;
            *pa = *pb;
            return GPARSE_OK;
        }

        case t_l64:{
            _l64_* pa = (_l64_*)a->pvalue;
            _l64_* pb = (_l64_*)b->pvalue;
            *pa = *pb;
            return GPARSE_OK;
        }

        case t_float:{
            _float_* pa = (_float_*)a->pvalue;
            _float_* pb = (_float_*)b->pvalue;
            *pa = *pb;
            return GPARSE_OK;
        }

        case t_double:{
            _double_* pa = (_double_*)a->pvalue;
            _double_* pb = (_double_*)b->pvalue;
            *pa = *pb;
            return GPARSE_OK;
        }

        default:
            return GPARSE_ERROR;
        }
    }
    else{
        switch (a->type){
        case t_bool:{
            _bool_* pa = (_bool_*)a->pvalue;
            _bool_ pb;
            if (variable_get_as( b, &pb ) != GPARSE_OK){
                return GPARSE_ERROR;
            }
            *pa = pb;
            return GPARSE_OK;
        }

        case t_byte:{
            _byte_* pa = (_byte_*)a->pvalue;
            _byte_ pb;
            if (variable_get_as( b, &pb ) != GPARSE_OK){
                return GPARSE_ERROR;
            }
            *pa = pb;
            return GPARSE_OK;
        }

        case t_int:{
            _int_* pa = (_int_*)a->pvalue;
            _int_ pb;
            if (variable_get_as( b, &pb ) != GPARSE_OK){
                return GPARSE_ERROR;
            }
            *pa = pb;
            return GPARSE_OK;
        }

        case t_l64:{
            _l64_* pa = (_l64_*)a->pvalue;
            _l64_ pb;
            if (variable_get_as( b, &pb ) != GPARSE_OK){
                return GPARSE_ERROR;
            }
            *pa = pb;
            return GPARSE_OK;
        }

        case t_float:{
            _float_* pa = (_float_*)a->pvalue;
            _float_ pb;
            if (variable_get_as( b, &pb ) != GPARSE_OK){
                return GPARSE_ERROR;
            }
            *pa = pb;
            return GPARSE_OK;
        }

        case t_double:{
            _double_* pa = (_double_*)a->pvalue;
            _double_ pb;
            if (variable_get_as( b, &pb ) != GPARSE_OK){
                return GPARSE_ERROR;
            }
            *pa = pb;
            return GPARSE_OK;
        }

        default:
            return GPARSE_ERROR;
        }
    }
}

/* Assign the value of a variable */
int variable_assign_numeric
    ( Numeric* num, const gVariable* _restrict_ const var )
{
    switch (var->type){
    case t_bool:{
        _bool_* pa = (_bool_*)var->pvalue;
        num->pool.vbool = *pa;
        break;
    }

    case t_byte:{
        _byte_* pa = (_byte_*)var->pvalue;
        num->pool.vbyte = *pa;
        break;
    }

    case t_int:{
        _int_* pa = (_int_*)var->pvalue;
        num->pool.vint = *pa;
        break;
    }

    case t_l64:{
        _l64_* pa = (_l64_*)var->pvalue;
        num->pool.vl64 = *pa;
        break;
    }

    case t_float:{
        _float_* pa = (_float_*)var->pvalue;
        num->pool.vfloat = *pa;
        break;
    }

    case t_double:{
        _double_* pa = (_double_*)var->pvalue;
        num->pool.vdouble = *pa;
        break;
    }

    default:
        return GPARSE_ERROR;
    }

    num->type = var->type;
    return GPARSE_OK;
}

#endif /* H_GVARIABLE_H */
