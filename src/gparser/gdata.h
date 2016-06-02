/***
Author: Mario J. Martin <dominonurbs$gmail.com>

*******************************************************************************/

#ifndef H_GDATA_H
#define H_GDATA_H

#include <stdint.h>

#define GPARSE_OK   0
#define GPARSE_ERROR     1
#define GPARSE_NO_COMMAND 2
#define GPARSE_NEW_NAME 0
#define GPARSE_NAME_COLLISION 1

enum OutMsgType
{
    msgtype_error = 0,
    msgtype_warning = 1,
};

/* Native types used in GLUUBS */
typedef bool            _bool_;
typedef unsigned char   _byte_;
typedef int             _int_;
typedef int64_t         _l64_;
typedef float          _float_;
typedef double         _double_;

enum BasicTypes
{
    t_undefined = 0,
    t_bool = 1,
    t_byte = -10,
    t_int = -11,
    t_l64 = -12,
    t_float = -21,
    t_double = -22,
};

#define TYPE(type) t_##type
#define t__bool_    t_bool
#define t__byte_    t_byte
#define t__int_     t_int
#define t__l64_     t_l64
#define t__float_   t_float
#define t__double_  t_double

typedef struct
{
    int type;
    int size;
    char* name;
    void* pvalue;
}gVariable;

/* Stores errors and warnings messages */
//typedef struct _gOutMsg
//{
//    int type;
//    char* msg;
//    _gOutMsg* next;
//    int line;       /* Indicates the poition where the line starts */
//    int column;     /* Indicates the column where the error is generated */
//}gOutMsg;

typedef struct
{
    int err_line;
    int err_column;
    char* err_msg;
    gVariable ans;

    /* Set to 0: default, 1: allow, -1: not allow */
    int explicit_declarations;
}gParser;

#endif /* H_GDATA_H */
