/***
Author: Mario J. Martin <dominonurbs$gmail.com>

Developping parser variable types and declarations
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

void check_separators()
{
    gParser* parser = gParser_create();

    check_parser( parser, "" ); printf( "\n" );
    check_parser( parser, ";" ); printf( "\n" );
    check_parser( parser, "1.0f" ); printf( "\n" );
    check_parser( parser, "1;2" ); printf( "\n" );
    check_parser( parser, "1 \n 3" ); printf( "\n" );
    check_parser( parser, "2 \\\n +2" ); printf( "\n" );
    check_parser( parser, "\\\n +5\r" ); printf( "\n" );
    check_parser( parser, "6;" ); printf( "\n" );
    check_parser( parser, "7;\r\n\r;\n" ); printf( "\n" );
    check_parser( parser, "7/*6\n*/+1" ); printf( "\n" );
    check_parser( parser, "9//+2" ); printf( "\n" );
    check_parser( parser, "7//\n+3" ); printf( "\n" );

    gParser_dispose( parser );
}

void check_casting()
{
    gParser* parser = gParser_create();

    check_parser( parser, "int 2.1" ); printf( "\t2i\n" );
    check_parser( parser, "int 1.0 + int 1.0" ); printf( "\t2i\n" );
    check_parser( parser, "double int 1.1" ); printf( "\t1.0d\n" );
    check_parser( parser, "int 1.1 + double 1.0" ); printf( "\t2d\n" );
    check_parser( parser, "float(true || true)" ); printf( "\t1f\n" );
    check_parser( parser, "0xFFFFFFFF01 + int64 0.0" );
    printf( "\t%lld(long)\n", 0xFFFFFFFF01L );
    check_parser( parser, "float(1.)" ); printf( "\t1f\n" );
    check_parser( parser, "float(1 + 1)" ); printf( "\t2f\n" );
    check_parser( parser, "(float)1" ); printf( "\t1f\n" );
    check_parser( parser, "(float)((1))" ); printf( "\t1f\n" );

    printf( "___errors___\n" );
    check_parser( parser, "1.0 + bool 1.1f" );
    check_parser( parser, "1 int" );; printf( "\t1f\n" );
    check_parser( parser, "float int" );; printf( "\t1f\n" );
    check_parser( parser, "float(1 + 1)int" );; printf( "\t1f\n" );
    check_parser( parser, "1 + 1 + int" ); printf( "\t1f\n" );
    check_parser( parser, "int 2.0 +" ); printf( "\t1f\n" );
    check_parser( parser, "()1" ); printf( "\t1f\n" );
    check_parser( parser, "((float))1" ); printf( "\t1f\n" );

    gParser_dispose( parser );
}

void check_variable_declaration()
{
    gParser* parser = gParser_create();

    printf( "\n" );
    check_parser( parser, "bool a = true; a" ); printf( "\n" );
    check_parser( parser, "int b = 1; b" ); printf( "\n" );
    check_parser( parser, "int ib = 1.0; ib" ); printf( "\n" );
    check_parser( parser, "int64 a0 = 1; a0" ); printf( "\n" );
    check_parser( parser, "float a1 = 1; a1" ); printf( "\n" );
    check_parser( parser, "double a3 = 1; a3" ); printf( "\n" );
    check_parser( parser, "byte f = a1; f" ); printf( "\n" );

    /* By default variables are initialized to 0 */
    check_parser( parser, "bool vbool; vbool" ); printf( "\n" );
    check_parser( parser, "int vint; vint" ); printf( "\n" );
    check_parser( parser, "double vdouble; vdouble" ); printf( "\n" );

    /* Optionally, implicit declarations is allowed,
    * but it is usually a big source of bugs. */
    check_parser( parser, "a4 = true; a4" ); printf( "\n" );
    check_parser( parser, "f = 1.0f; f" ); printf( "\n" );
    check_parser( parser, "f0 = f1 = f; f0" ); printf( "\n" );
    check_parser( parser, "f0 = (f1 = f); f0" ); printf( "\n" );
    check_parser( parser, "int ib0 = int true; ib0" ); printf( "\n" );

    /* We can force that variables should be explicitly declared. */
    parser->option_explicit_decl = 1;
    check_parser( parser, "int f00 = 1" ); printf( "\n" );
    check_parser( parser, "bool b00 = bool true; b00" ); printf( "\n" );
    check_parser( parser, "f00 = int true; f00" ); printf( "\tok\n" );

    /* Multiple declarations separated by commas */
    check_parser( parser, "int c0, c1 = 1, c2, c3 = 3.0" ); printf( "\n" );
    check_parser( parser, "c0" ); printf( "\t0\n" );
    check_parser( parser, "c1" ); printf( "\t1\n" );
    check_parser( parser, "c2" ); printf( "\t0\n" );
    check_parser( parser, "c3" ); printf( "\t3\n" );

    printf( "___errors___\n" );
    parser->option_explicit_decl = 0;
    check_parser( parser, "f0 = (f1 = err); f0" ); printf( "\n" );

    parser->option_explicit_decl = 1;
    check_parser( parser, "undeclared = false; undeclared" ); printf( "\n" );
    check_parser( parser, "f0 = true; f0" ); printf( "...> f0 is already declared as float\n" );
    check_parser( parser, "f0 = err = 1; b" ); printf( "\n" );
    check_parser( parser, "int c0, c0 = 1" ); printf( "\n" );
    check_parser( parser, "int g123, g123 = 1" ); printf( "\n" );
    check_parser( parser, "int aa = 1, c0 = 1" ); printf( "\n" );

    gParser_dispose( parser );
}

void check_add_variable()
{
    double x = 2;

    gParser* parser = gParser_create();
    gVariable* var_x = gParser_addVariable( parser, "_x_", t_double, &x );
    display_var( *var_x );
    printf( "\n" );
    gVariable* var = gParser_findVariable( parser, "_x_" );

    int status = gParser_command( parser, "double _y_" );
    gVariable* var_y = gParser_findVariable( parser, "_y_" );

    if (var_y->type == t_double){
        double* py = (double*)var_y->pvalue;
        *py = 2;
    }
    display_var( *var_y );
    printf( "\n" );

    gParser_dispose( parser );
}

void check_struct()
{
    gParser* parser = gParser_create();

    printf( "\n" );
    check_parser( parser, "struct Vector{ double x, y, z }" );
    gParser_dispose( parser );
}

int main( int argc, char* argv[] )
{
    clock_t init = clock();

    //check_separators();
    //check_casting();
    check_variable_declaration();
    check_add_variable();

    clock_t end = clock();
    printf( "time:%i", end - init );
    _CrtDumpMemoryLeaks();

    getchar();

    return 0;
}
