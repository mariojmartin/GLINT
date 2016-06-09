
#include <stdio.h>
#include "gparser/gparser.h"

/* Values in the variable is in the .pvalue field, 
* which can point to different data types.
* Check .type and cast to the correct variable 
* The following function just do that. */
static void display_var( const gVariable* var )
{
    if (var == nullptr){
        printf( "(null)" );
        return;
    }

    switch (var->type){
        case t_undefined:{  /* The variable is not set */
            printf( "(undefined)" );
            break;
        }
        case t_bool:{       /* Boolean type */
            char* p = (char*)var->pvalue;
            if (*p){
                printf( "true" );
            }
            else{
                printf( "false" );
            }
            break;
        }
        case t_byte:{
            _byte_* p = (_byte_*)var->pvalue;
            printf( "(byte)%u", *p );
            break;
        }
        case t_int:{        /* Equivalent to 'int' in C */
            int* p = (int*)var->pvalue;
            printf( "%ii", *p );
            break;
        }
        case t_double:{     /* Equivalent to 'double' in C */
            double* p = (double*)var->pvalue;
            printf( "%.4ed", *p );
            break;
        }
        case t_l64:{        /* integer of 64 bits */
            _l64_* p = (_l64_*)var->pvalue;
            printf( "%lldL", *p );
            break;
        }
        case t_float:{      /* Equivalent to 'float' in C */
            float* p = (float*)var->pvalue;
            printf( "%.4ff", *p );
            break;
        }

        default:
            ; /* Do nothing */
    }
}

/* To simply parser a string only two functions are needed. */
void simple_example()
{
    char command[] = "1+2+3";

    /* First create a parser */
    gParser* parser = gParser_create();

    /* Calculate the aritmetic operations in the string using the parser */
    int status = gParser_command( parser, command );

    /* Check the results */
    if (status == GPARSE_OK){
        /* The solution of the operation is stored in ans */ 
        /* To directly obtain the value as a double */
        double v = gVariable_getasDouble( parser->ans );
        printf( "    %g", v );
    }
    else{ /* (status == GPARSE_ERROR) There is an error */
        printf( "%s", command );

        /* Shows the error message.*/
        printf( "\n%s. ", parser->err_msg );

        /* Indicates the token that has triggered the error. */
        printf( "char: %i ", parser->err_column );
    }
    printf( "\n" );

    /* Finally release memory resources */
    gParser_dispose( parser );
}

void variable_examples()
{
    int status;

    /* Create a parser */
    gParser* parser = gParser_create();

    /* Create a variable and attach to an external memory region.
     * In this case the memory location is handled by the programer.
     * Be aware that in this case, the scope of x ends with the function.
     * Take care that the parser points to an valid data location and 
     * to free the memory if it is created in the heap with new or malloc
     * in order to avoid memory leaks. */
    double x = 0;
    gVariable* var_x = gParser_addVariable( parser, "x", t_double, &x );
    x = 3.14;
    display_var( var_x );
    printf( "\n" );

    /* We can also declare a variable and get the memory location.
     * In this case the data is a memory location handled by the parser
     * and it is not longer valid after gParser_dispose() */
    status = gParser_command( parser, "double y" );
    gVariable* var_y = gParser_findVariable( parser, "y" );
    double* py = (double*)var_y->pvalue;
    *py = 3.14;
    display_var( var_y );
    printf( "\n" );

    /* Glubs encorages explicit declarations as a good practice to avoid bugs,
     * but implicit declarations are allowed if the option is activated */
    parser->option_explicit_decl = 1;
    status = gParser_command( parser, "z = 3.14" );
    gVariable* var_z = gParser_findVariable( parser, "z" );
    display_var( var_z );
    printf( "\n" );

    gParser_dispose( parser );
}

/* To simply parser a string only two functions are needed. */
int main( int argc, char* argv[] )
{
    simple_example();
    variable_examples();

    getchar();
}