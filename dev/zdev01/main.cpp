/***
Author: Mario J. Martin <dominonurbs$gmail.com>

Developping parser numeric aritmetic expressions
*******************************************************************************/

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include <stdint.h>

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

void check_parser_char2num()
{
    gParser* parser = gParser_create();

    check_parser( parser, "0" );    printf( "\t0i\n" );
    check_parser( parser, "1" );    printf( "\t1i\n" );
    check_parser( parser, "10" );   printf( "\t10i\n" );
    check_parser( parser, "19" );   printf( "\t19i\n" );
    check_parser( parser, "019" );  printf( "\t19i\n" );
    check_parser( parser, "0190" ); printf( "\t190i\n" );

    gParser_dispose( parser );
}

void check_parser_unitary_sign()
{
    gParser* parser = gParser_create();

    check_parser( parser, "+1" );       printf( "\t1i\n" );
    check_parser( parser, " - 1 " );    printf( "\t-1i\n" );
    check_parser( parser, " +-3 " );    printf( "\t-3i\n" );
    check_parser( parser, " - +3 " );   printf( "\t-3i\n" );
    check_parser( parser, " - - +3 " ); printf( "\t3i\n" );

    gParser_dispose( parser );
}

void check_parser_plus_minus()
{
    gParser* parser = gParser_create();

    check_parser( parser, " 1 + 2 " );      printf( "\t3i\n" );
    check_parser( parser, " 1 + 1 + 01 " ); printf( "\t3i\n" );
    check_parser( parser, " +1+1+ +1" );    printf( "\t3i\n" );
    check_parser( parser, " 4 - 1" );       printf( "\t3i\n" );
    check_parser( parser, " 6 - 1 - 2" );   printf( "\t3i\n" );
    check_parser( parser, " -1 + 4" );      printf( "\t3i\n" );
    check_parser( parser, " 1 + 2 - 3 -+-+ 3" ); printf( "\t3i\n" );

    gParser_dispose( parser );
}

void check_parser_mul_div()
{
    gParser* parser = gParser_create();

    /* It is solved from left to right */
    check_parser( parser, "2*3" );      printf( "\t6i\n" );
    check_parser( parser, "4/2" );      printf( "\t2d\n" );
    check_parser( parser, "4/2/2" );    printf( "\t1d\n" );
    check_parser( parser, "2*3/2" );    printf( "\t3d\n" );
    check_parser( parser, "2*3/2*2" );  printf( "\t6d\n" );
    check_parser( parser, "1+8/2-2" );  printf( "\t3d\n" );

    gParser_dispose( parser );
}

void check_parser_mul_plus()
{
    gParser* parser = gParser_create();

    /* It is solved from left to right */
    check_parser( parser, "2*+3" );      printf( "\t6i\n" );
    check_parser( parser, "-4/-2" );      printf( "\t2d\n" );
    check_parser( parser, "4/+-+-2/2" );    printf( "\t1d\n" );

    gParser_dispose( parser );
}

void check_parser_decimal()
{
    gParser* parser = gParser_create();

    check_parser( parser, "0." );       printf( "\t0d\n" );
    check_parser( parser, "3.1416" );   printf( "\t3.1416d\n" );
    check_parser( parser, "2.0*3.0" );  printf( "\t6d\n" );
    check_parser( parser, "2.*3" );     printf( "\t6d\n" );
    check_parser( parser, "2*3." );     printf( "\t6d\n" );
    check_parser( parser, "2.+4." );    printf( "\t6d\n" );
    check_parser( parser, "2+4." );     printf( "\t6d\n" );
    check_parser( parser, "2.+4" );     printf( "\t6d\n" );
    check_parser( parser, "2.-4." );    printf( "\t-2d\n" );
    check_parser( parser, "2-4." );     printf( "\t-2d\n" );
    check_parser( parser, "2.-4" );     printf( "\t-2d\n" );
    check_parser( parser, "3./2" );     printf( "\t1.5d\n" );
    check_parser( parser, "3/2." );     printf( "\t1.5d\n" );
    check_parser( parser, "3./2." );    printf( "\t1.5d\n" );

    /* Different from C, divisions always cast to floating point */
    check_parser( parser, "3/2" );      printf( "\t1.5\n" );

    /* Integer division is indicated by %% */
    check_parser( parser, "5%%2" );   printf( "\t2i\n" );
    check_parser( parser, "5.0%%2" );   printf( "\t2i\n" );
    check_parser( parser, "5%%2.0" );   printf( "\t2i\n" );
    check_parser( parser, "5.0%%2.0" );   printf( "\t2i\n" );
    check_parser( parser, "5L%%2L" );   printf( "\t2L\n" );

    /* Remainder */
    check_parser( parser, "5%2" );   printf( "\t1i\n" );

    gParser_dispose( parser );
}

void check_parser_bool()
{
    gParser* parser = gParser_create();

    check_parser( parser, "true" );                         printf( "\ttrue\n" );
    check_parser( parser, "false" );                        printf( "\tfalse\n" );
    check_parser( parser, "false || true && true && false" );  printf( "\tfalse\n" );
    gParser_dispose( parser );
}

void check_parser_float()
{
    gParser* parser = gParser_create();
    check_parser( parser, "3.14 " );            printf( "\t3.14d\n" );
    check_parser( parser, "3.14f " );           printf( "\t3.14f\n" );
    check_parser( parser, "314e2 " );           printf( "\t31400d\n" );
    check_parser( parser, "314e0" );            printf( "\t314d\n" );
    check_parser( parser, "314e2f " );          printf( "\t314f\n" );
    check_parser( parser, "3.14e000 " );        printf( "\t314d\n" );
    check_parser( parser, "3.14ef " );          printf( "\t3.14f\n" );
    check_parser( parser, "0.0314e+2 " );       printf( "\t3.14d\n" );
    check_parser( parser, "314e-2 " );          printf( "\t3.14d\n" );
    check_parser( parser, "314e-2f" );          printf( "\t3.14f\n" );
    check_parser( parser, "0.0314e1f*10" );     printf( "\t3.14f\n" );
    check_parser( parser, "10*0.00314e2f" );    printf( "\t3.14f\n" );
    check_parser( parser, "10.*0.00314e2f" );   printf( "\t3.14d\n" );
    check_parser( parser, "0.00314e2f*10." );   printf( "\t3.14d\n" );
    check_parser( parser, "0.00314e2f*10f" );   printf( "\t3.14f\n" );
    check_parser( parser, "0.000000000001" );   printf( "\t1e-12\n" );
    check_parser( parser, "0.000000000001f" );  printf( "\t0f Precision error?\n" );
    check_parser( parser, "0.000000000001e12" ); printf( "\t1d\n" );
    check_parser( parser, "1000000000000e-12f" ); printf( "\t1f\n" );
    check_parser( parser, "300e-2*0.2e1/1e0f" ); printf( "\t6\n" );
    check_parser( parser, "300e-2*0.2e1f/1e0" ); printf( "\t6\n" );
    check_parser( parser, "300e-2f*0.2e1f/1e0f" ); printf( "\t6f\n" );

    gParser_dispose( parser );
}

void check_parser_long()
{
    gParser* parser = gParser_create();
    check_parser( parser, "1L" ); printf( "\t1L\n" );
    check_parser( parser, "0xFFFFFFFF01" ); printf( "\t%lld(long)\n", 0xFFFFFFFF01 );
    check_parser( parser, "1L + 1" ); printf( "\t2L\n" );
    check_parser( parser, "1L + 1.0" ); printf( "\t2d\n" );
    check_parser( parser, "1L + 1.0f" ); printf( "\t2f\n" );
    check_parser( parser, "1 + 1L" ); printf( "\t2L\n" );
    check_parser( parser, "1.0 + 1L" ); printf( "\t2d\n" );
    check_parser( parser, "1.0f + 1L" ); printf( "\t2f\n" );

    gParser_dispose( parser );
}

void check_pow()
{
    gParser* parser = gParser_create();
    check_parser( parser, "2**2" );      printf( "\t4d\n" );
    check_parser( parser, "2^2" );      printf( "\t4d\n" );
    check_parser( parser, "2**-2" );     printf( "\t0.25d\n" );
    check_parser( parser, "2f**-2" );    printf( "\t0.25f\n" );
    check_parser( parser, "2**-2f" );    printf( "\t0.25f\n" );
    check_parser( parser, "2L**-2f" );    printf( "\t0.25f\n" );
    check_parser( parser, "2f**-2f" );   printf( "\t0.25f\n" );
    check_parser( parser, "2**-2" );    printf( "\t0.25d\n" );
    check_parser( parser, "2L**-2" );    printf( "\t0.25d\n" );
    check_parser( parser, "2.5*1**-2f" ); printf( "\t0.25d\n" );

    gParser_dispose( parser );
}

void check_round_bracket()
{
    gParser* parser = gParser_create();
    check_parser( parser, "(2)" );               printf( "\t2i\n" );
    check_parser( parser, "(2+2)" );             printf( "\t4i\n" );
    check_parser( parser, "2*(2+2)" );           printf( "\t8i\n" );
    check_parser( parser, "2*((2+2)*(2-3))" );   printf( "\t-8i\n" );
    check_parser( parser, "2*(((2+2)*(2-3)))" ); printf( "\t-8i\n" );
    check_parser( parser, "(2)*((2+2)*(2-3))" ); printf( "\t-8i\n" );
    check_parser( parser, "-2*(2+2)+2-2" ); printf( "\t-8i\n" );

    check_parser( parser, "1 + 2*(((2+2)*(2-3)) + 1" ); printf( "\tunmatching brackets!\n" );
    check_parser( parser, "1 + ((2)*((2+2)*(2-3)) + 1" ); printf( "\tunmatching brackets!\n" );
    check_parser( parser, "1 + (2))*((2+2)*(2-3)) + 1" ); printf( "\tunmatching brackets!\n" );

    gParser_dispose( parser );
    _CrtDumpMemoryLeaks();
}

void check_comparisons()
{
    gParser* parser = gParser_create();
    check_parser( parser, "3==2" );             printf( "\tfalse\n" );
    check_parser( parser, "3!=2" );             printf( "\ttrue\n" );
    check_parser( parser, "3==2" );             printf( "\tfalse\n" );
    check_parser( parser, "3>2" );              printf( "\ttrue\n" );
    check_parser( parser, "3<2" );              printf( "\tfalse\n" );
    check_parser( parser, "3>=2" );             printf( "\ttrue\n" );
    check_parser( parser, "3>=3" );             printf( "\ttrue\n" );
    check_parser( parser, "3<=2" );             printf( "\tfalse\n" );
    check_parser( parser, "3<=3" );             printf( "\ttrue\n" );
    check_parser( parser, "3.0000000001>3" ); printf( "\ttrue\n" );
    check_parser( parser, "3<3.0000000001" ); printf( "\tfalse\n" );
    check_parser( parser, "3<3.0000001f" ); printf( "\tfalse Precision error?\n" );
    check_parser( parser, "1f/3==1f/3" );       printf( "\ttrue\n" );
    check_parser( parser, "1./3==1./3" );       printf( "\ttrue\n" );
    check_parser( parser, "true!=false" );      printf( "\ttrue\n" );
    check_parser( parser, "3==1+1+1" );         printf( "\ttrue\n" );
    check_parser( parser, "3.1f==3.1" );         printf( "\tfalse\n" );

    gParser_dispose( parser );
}

void check_booleans()
{
    gParser* parser = gParser_create();
    check_parser( parser, "true || true && false" );    printf( "\ttrue\n" );
    check_parser( parser, "(true || true) && false" );  printf( "\tfalse\n" );
    check_parser( parser, "3==2+1 && 4>=2*2" );         printf( "\ttrue\n" );

    check_parser( parser, "1 || false" ); printf( "\t(err)\n" );

    gParser_dispose( parser );
}

void check_bitoperands()
{
    gParser* parser = gParser_create();
    check_parser( parser, "0xFFF0F0F0 & 0x33333333" );
    printf( "\t%i\n", 0xFFF0F0F0 & 0x33333333 );

    check_parser( parser, "0xFFF0F0F0 xor 0x33333333" );
    printf( "\t%u\n", 0xFFF0F0F0 ^ 0x33333333 );

    check_parser( parser, "0xFFF0F0F0 | 0x33333333" );
    printf( "\t%u\n", 0xFFF0F0F0 | 0x33333333 );

    /* Binary literals can be written using the notation 0b... */
    check_parser( parser, "0b0011 | 0b0100" );
    printf( "\t%u\n", 3 | 4 );

    check_parser( parser, "1<<2" );
    printf( "\t%i\n", 1 << 2 );
    check_parser( parser, "4>>2" );
    printf( "\t%i\n", 4 >> 2 );

    /* This one is awkward, but in C '+' has precedence over '<<' */
    check_parser( parser, "1+4<<2" );
    printf( "\t%i\n", (1 + 4) << 2 );

    gParser_dispose( parser );
}

void check_errors()
{
    gParser* parser = gParser_create();

    check_parser( parser, "" );
    check_parser( parser, " +" );
    check_parser( parser, " +-- " );
    check_parser( parser, " 5 + 5 +* 5" );

    check_parser( parser, "1 + 2 + true + 3" );
    check_parser( parser, "1 + 3 + true * false + 4" );
    check_parser( parser, "1 + 2 + 3 ++ 4" );
    check_parser( parser, "1 + 2 + a + 1" );

    check_parser( parser, "true && 2" );
    check_parser( parser, "1 + true * 2 + 1" );
    check_parser( parser, "1 & 2 & true & 2" );
    check_parser( parser, "1 | false" );

    check_parser( parser, "1 << 3.1" );
    check_parser( parser, "* 1 * 3" );
    check_parser( parser, "1 + 2 * " );

    gParser_dispose( parser );
}

int main( int argc, char* argv[] )
{
    clock_t init = clock();

    check_parser_char2num();
    check_parser_unitary_sign();
    check_parser_plus_minus();
    check_parser_mul_div();
    check_parser_mul_plus();
    check_parser_decimal();
    check_parser_bool();
    check_parser_float();
    check_parser_long();
    check_pow();
    check_round_bracket();
    check_comparisons();
    check_booleans();
    check_bitoperands();
    check_errors();
    clock_t end = clock();
    printf( "time:%i", end - init );

    _CrtDumpMemoryLeaks();
    getchar();
    return 0;
}

