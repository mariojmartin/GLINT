/***
Author: Mario J. Martin <dominonurbs$gmail.com>

Developping structures and function declarations
*******************************************************************************/

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <stdlib.h>

#include "gparser/gparser.h"

void display_var( const gVariable var )
{
    switch (var.type){
    case t_undefined:{
        printf( "(null)" );
        break;
    }
    case t_bool:{
        char* p = (char*)var.pvalue;
        if (*p){
            printf( "true" );
        }
        else{
            printf( "false" );
        }
        break;
    }
    case t_byte:{
        _byte_* p = (_byte_*)var.pvalue;
        printf( "(byte)%u", *p );
        break;
    }
    case t_int:{
        int* p = (int*)var.pvalue;
        printf( "%ii", *p );
        break;
    }
    case t_l64:{
        _l64_* p = (_l64_*)var.pvalue;
        printf( "%lldL", *p );
        break;
    }
    case t_float:{
        float* p = (float*)var.pvalue;
        printf( "%.4ff", *p );
        break;
    }
    case t_double:{
        double* p = (double*)var.pvalue;
        printf( "%.4ed", *p );
        break;
    }
    default:
        ; /* Do nothing */
    }
}

int check_parser( gParser* parser, const char* code )
{
    int status = gParser_command( parser, code );
    if (status != GPARSE_ERROR){
        display_var( parser->ans );
    }
    else{
        char spaces[] = "                               ";
        spaces[parser->err_column] = '\0';
        printf( "%s \n%s%s%s\n", code, spaces, "^->", parser->err_msg );
    }

    return status;
}

void check_struct()
{
    gParser* parser = gParser_create();

    printf( "\n" );
    check_parser( parser, "struct Vector{ double x, y, z }" );
    gParser_dispose( parser );
}

void check_function()
{
    gParser* parser = gParser_create();

    printf( "\n" );
    check_parser( parser, "function int myfunc(int a, int b)"
        "{ int c = a+b; return c}" );
    gParser_dispose( parser );
}
int main( int argc, char* argv[] )
{
    clock_t init = clock();

    //check_struct();
    check_function();

    clock_t end = clock();
    printf( "time:%i", end - init );
    _CrtDumpMemoryLeaks();
    getchar();

    return 0;
}
