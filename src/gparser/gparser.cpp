/**
Author: Mario J. Martin <dominonurbs$gmail.com>

The parser first identifies the tokens in the string, 
then solves the numeric operations.

The notation is identical to C language.
*******************************************************************************/

#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <math.h>

#include "common/definitions.h"

#include "gdata.h"
#include "Numeric.hpp"
#include "Variable.hpp"
#include "Parser.hpp"

/* Predeclaration of functions */
int parse_command( Numeric* ans, Parser* parser, Struct* strwct
    , const Token* const _restrict_ tok_left
    , const Token* const _restrict_ tok_right );

int parser_code( Parser* parser, Struct* str
    , const char* code_ini, const char* code_end );

/*******************************/

static int str2num( Numeric* ans, const Token* const token )
{
    enum{ token_integer, token_decimal
        , token_exponent, token_float, token_long
        , token_hexadecimal, token_binary
        };

    register const char* p = token->str_ini;
    register long long ivalue = 0;
    register long long frac = 0;
    register long dec = 0;
    register long exp = 0;
    register int exp_sign = 1;
    int hex;
    int token_type = token_integer;

    /* Clean whitespaces at the beggining */
    while (*p == ' ' || *p == '\t')  {
        p++;
    }

    if (*p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X')){
        /* It is an hexadecimal */
        token_type = token_hexadecimal;
        p += 2;
    }
    else if (*p == '0' && (*(p + 1) == 'b' || *(p + 2) == 'B')){
        /* It is an binary */
        token_type = token_binary;
        p += 2;
    }

    while (p < token->str_end){
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r'){
            break;
        }
        if (token_type == token_hexadecimal){
            if (*p >= '0' && *p <= '9'){
                hex = *p - '0';
            }
            else if (*p >= 'A' && *p <= 'F'){
                hex = *p - 'A' + 10;
            }
            else if (*p >= 'a' && *p <= 'f'){
                hex = *p - 'a' + 10;
            }
            else{
                return GPARSE_ERROR;
            }
            ivalue <<= 4;
            ivalue += hex;
            p++;
        }
        else if (token_type == token_binary){
            if (*p >= '0' && *p <= '1'){
                ivalue <<= 1;
                ivalue += (*p == '1') ? 1 : 0;
                p++;
            }
        }
        else if (*p >= '0' && *p <= '9'){
            if (token_type == token_integer){
                ivalue *= 10;
                ivalue += (*p - '0');
                p++;
            }
            else if (token_type == token_decimal){
                frac *= 10;
                frac += (*p - '0');
                p++;
                dec--;
            }
            else if (token_type == token_exponent){
                exp *= 10;
                exp += (*p - '0');
                p++;
            }
            else{
                return GPARSE_ERROR;
            }
        }
        else if (*p == '.'){
            if (token_type == token_integer){
                token_type = token_decimal;
                p++;
            }
            else{
                /* There are two dots */
                return GPARSE_ERROR;
            }
        }
        else if (*p == 'e' || *p == 'E'){
            token_type = token_exponent;
            p++;
            if (*p == '+'){
                p++;
            }
            else if (*p == '-'){
                p++;
                exp_sign = -1;
            }
        }
        else if (*p == 'f' || *p == 'F'){
            token_type = token_float;
            p++;
        }
        else if (*p == 'l' || *p == 'L'){
            token_type = token_long;
            p++;
        }
        else{
            /* Not a number */
            return GPARSE_ERROR;
        }
    }

    if (token_type == token_hexadecimal || token_type == token_binary){
        if (ivalue > INT_MAX){
            token_type = token_long;
        }
        else{
            token_type = token_integer;
        }
    }

    if (token_type == token_integer){
        /* Assign an integer */
        numeric_set( ans, (int)ivalue );
    }
    else if (token_type == token_long){
        /* Assign an integer */
        numeric_set( ans, _l64_(ivalue) );
    }
    else{
        /* Assign as floating point */
        double f = (double)ivalue;
        if (frac != 0){
            f += frac * pow( 10.0, dec );
        }
        if (exp != 0){
            exp = exp_sign > 0 ? exp : -exp;
            f *= pow( 10.0, exp );
        }
        if (token_type == token_float){
            /* Assign as float */
            numeric_set( ans, _float_(f) );
        }
        else{
            /* Assign as double */
            numeric_set( ans, _double_(f) );
        }
    }

    return GPARSE_OK;
}

#define LEFT2RIGHT 0
#define RIGHT2LEFT 1
static const Token* get_dual_operand
( Parser* parser
, const Token* const tok_ini
, const Token* const tok_end
, const unsigned int token_type
, const int left2right      
, int* status
)
{
    register const Token* p = left2right ? tok_end - 1 : tok_ini + 1;
    const Token* const p0 = left2right ? tok_end : tok_ini;
    const Token* const p1 = left2right ? tok_ini : tok_end;
    int bracket = 0;

    /* Check the brackets in the first token */
    switch (p0->token_type){
    case token_bracket_round_close:
        bracket--;
        break;
    case token_bracket_round_open:
        bracket++;
        break;
    }

    /* Find a dual operand */
    while (p > tok_ini && p < tok_end){
        switch (p->token_type){
        case token_bracket_round_close:
            bracket--;
            break;
        case token_bracket_round_open:
            bracket++;
            break;
        default:
            if (bracket == 0 && (p->token_type & token_type) != 0){
                /* The rule is that dual operators must have a variable, literal
                 * or closing bracket at the left side */
                const Token* pL = p - 1;
                if ((pL->token_type & NAME_MASK) != 0
                    || pL->token_type == token_bracket_round_close){
                    *status = GPARSE_OK;
                    return p;
                }
            }
        }
        if (left2right != 0){
            p--;
        }
        else{
            p++;
        }
    }

    /* Check the brackets in the last token*/
    switch (p1->token_type){
    case token_bracket_round_close:
        bracket--;
        break;
    case token_bracket_round_open:
        bracket++;
        break;
    }

    if (bracket > 0){
        *status = GPARSE_ERROR;
        parser_error( parser, "Unmatching bracket" );
        parser->code_pos = tok_end->str_end;
        return nullptr;
    }
    if (bracket < 0){
        *status = GPARSE_ERROR;
        parser_error( parser, "Unmatching bracket" );
        parser->code_pos = tok_ini->str_ini;
        return nullptr;
    }

    /* There is no command */
    *status = GPARSE_NO_COMMAND;
    return nullptr;
}

static int unary_operands
    ( Numeric* ans
    , Parser* parser, Struct* strwct
    , const Token* _restrict_ const tok_ini
    , const Token* _restrict_ const tok_end
    )
{
    int status = parse_command( ans, parser, strwct, tok_ini + 1, tok_end );
    if (status){
        return status;
    }

    switch (tok_ini->token_type){
    case token_plus: /* ans = +ans */
        /* The '+' do nothing, but expects a numeric value */
        if (numeric_plus( ans, parser )){
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_minus: /* ans = -ans; */ 
        if (numeric_neg( ans, parser )){
            parser->code_pos = tok_end->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_not: /* ans = !ans */
        if (numeric_not( ans, parser )){
            parser->code_pos = tok_end->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_bitinv: /* ans = ~ans */
        if (numeric_bitinv( ans, parser )){
            parser->code_pos = tok_end->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    case token_vartype: /* casting (convert one type to another) */
        if (numeric_explicit_cast( ans, tok_ini->var_type )){
            parser_error( parser, "Cannot cast" );
            parser->code_pos = tok_end->str_ini;
        }
        break;

    default:
        parser_error( parser, "Unknown operation" );
        return GPARSE_ERROR;
    }

    return GPARSE_OK;
}

static const int bracket
    ( Numeric* ans
    , Parser* parser, Struct* strwct
    , const Token* const tok_ini
    , const Token* const tok_end
    )
{
    if (tok_ini->token_type == token_bracket_round_open){
        /* Find an close bracket */
        int ibr = 1;
        const Token* p = tok_ini + 1;
        while (p <= tok_end){
            switch (p->token_type){
            case token_bracket_round_open:
                ibr++;
                break;
            case token_bracket_round_close:
                ibr--;
                break;
            }
            if (ibr == 0){
                if (p != tok_end){
                    parser_error( parser, "Expression error" );
                    parser->code_pos = (p + 1)->str_ini;
                    return GPARSE_ERROR;
                }
                return parse_command( ans, parser, strwct, tok_ini + 1, p - 1 );
            }
            else if (ibr < 0){
                parser_error( parser, "Unmatching bracket" );
                parser->code_pos = p->str_ini;
                return GPARSE_ERROR;
            }
            p++;
        }
        parser_error( parser, "Unmatching bracket" );
        parser->code_pos = tok_end->str_ini;
        return GPARSE_ERROR;
    }
    else{
        return GPARSE_NO_COMMAND;
    }
}

static int unary_operation
    ( Numeric* ans
    , Parser* parser, Struct* strwct
    , const Token* _restrict_ const tok_ini
    , const Token* _restrict_ const tok_end
    , const int operator_mask
    )
{
    if ( (tok_ini->token_type & operator_mask) != 0){
        if (tok_ini < tok_end){
            return unary_operands( ans, parser, strwct, tok_ini, tok_end );
        }
        else{
            parser_error( parser, "Expecting expression" );
            parser->code_pos = tok_ini->str_end;
            return GPARSE_ERROR;
        }
    }

    return GPARSE_NO_COMMAND;
}

static int dual_operation
    ( Numeric* ans
    , Parser* parser, Struct* strwct
    , const Token* _restrict_ const tok_ini
    , const Token* _restrict_ const tok_end
    , const int operator_mask 
    )
{
    Numeric a;
    Numeric b;
    int status;

    const Token* op = get_dual_operand
        ( parser, tok_ini, tok_end, operator_mask, RIGHT2LEFT, &status );

    if (op == nullptr){
        /* The dual operator is not present or there is an error */
        return status;
    }

    /* Gets the left operand */
    status = parse_command( &a, parser, strwct, tok_ini, op - 1 );
    if (status) return status;

    /* Gets the right operand */
    status = parse_command( &b, parser, strwct, op + 1, tok_end );
    if (status) return status;

    numeric_copy( ans, a );

    switch (op->token_type){
    case token_plus: /*ans = a + b */
        if (numeric_add( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_minus: /* ans = a - b */
        if (numeric_sub( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_mul: /* ans = a * b */
        if (numeric_mul( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_div: /* ans = a / b */
        if (numeric_div( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_remainder: /* ans = a % b */
        if (numeric_remainder( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_intdiv: /* ans = a %% b */
        if (numeric_intdiv( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_pow: /* ans = a ^ b */
        if (numeric_pow( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_equal: /* ans = a == b */
        if (numeric_equal( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_notequal: /* ans = a != b */
        if (numeric_notequal( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_greater: /* ans = a > b */
        if (numeric_greater( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_greaterequal: /* ans = a >= b */
        if (numeric_greater_equal( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_less: /* ans = a < b */
        if (numeric_less( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_lessequal: /* ans = a <= b */
        if (numeric_less_equal( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_and: /* ans = a && b */
        if (numeric_and( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_or: /* ans = a || b */
        if (numeric_or( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;
    case token_bitand: /* ans = a || b */
        if (numeric_bit_and( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_bitxor: /* ans = a || b */
        if (numeric_bit_xor( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_bitor: /* ans = a || b */
        if (numeric_bit_or( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_lshift: /* ans = a || b */
        if (numeric_lshift( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    case token_rshift: /* ans = a || b */
        if (numeric_rshift( ans, &b, parser )){
            parser->code_pos = op->str_ini;
            return GPARSE_ERROR;
        }
        return GPARSE_OK;

    default:
        parser_error( parser, "Unknown operation" );

        return GPARSE_ERROR;
    }
}

static int assign_operation
( Numeric* ans, Parser* parser, Struct* strwct
, const Token* _restrict_ const tok_ini
, const Token* _restrict_ const tok_end
)
{
    int status;

    const Token* op = get_dual_operand
        ( parser, tok_ini, tok_end, ASSIGN_MASK, LEFT2RIGHT, &status );

    if (op == nullptr){
        if (status == GPARSE_ERROR){
            parser->code_pos = tok_ini->str_ini;
        }
        return status;
    }

    /* Gets the left variable */
    if (parser->explicit_declarations != 0){
        Variable* var = strwct->find_variable( tok_ini->str_ini, tok_ini->str_end );
        if (var == nullptr){
            parser_error( parser, "Undeclared variable" );
            parser->code_pos = tok_ini->str_ini;
            return GPARSE_ERROR;
        }

        /* Calculate the right term. */
        status = parse_command( ans, parser, strwct, op + 1, tok_end );
        if (status != GPARSE_OK) return status;

        status = variable_assign( var, ans );
        if (status != GPARSE_OK){
            parser_error( parser, "Cannot perform implicit casting" );
            parser->code_pos = op->str_ini;
            return status;
        }

        /* Cast the right term if possible */
        status = numeric_implicit_cast( ans, var->type );
        if (status != GPARSE_OK){
            parser_error( parser, "Cannot perform implicit casting" );
            parser->code_pos = op->str_ini;
            return status;
        }

        /* Copy the right value to the variable */
        variable_assign( var, ans );
        return GPARSE_OK;
    }
    else{ /* Implicit declarations allowed */
        Variable* var = strwct->add_variable
            ( tok_ini->str_ini, tok_ini->str_end, &status );

        /* Calculate the right term */
        status = parse_command( ans, parser, strwct, op + 1, tok_end );
        if (status != GPARSE_OK) return status;

        /* Assign and cast the variable to the right term */
        variable_dynamic_assign( var, ans );
    }

    return GPARSE_OK;
}

int parse_command( Numeric* ans, Parser* parser, Struct* strwct
    , const Token* _restrict_ const tok_ini
    , const Token* _restrict_ const tok_end )
 {
    int status;

    if (tok_ini > tok_end){
        parser_error( parser, "Expecting an expresion" );
        return GPARSE_ERROR;
    }
    if (tok_ini == tok_end){
        switch (tok_ini->token_type){
        case token_literal_number:
            status = str2num( ans, tok_end );
            if (status){
                parser_error( parser, "Expecting an expresion" );
                parser->code_pos = tok_ini->str_ini;
                return GPARSE_ERROR;
            }
            return GPARSE_OK;
        case token_name:{
            /* Find the variable */
            Variable* var = strwct->find_variable
                ( tok_ini->str_ini, tok_ini->str_end );
            if (var != nullptr){
                variable_assign_numeric( ans, var );
            }
            else{
                parser_error( parser, "Undeclared variable" );
                parser->code_pos = tok_ini->str_ini;
                return GPARSE_ERROR;
            }
            return GPARSE_OK;
        }
        case token_literal_true:
            numeric_set( ans, _bool_(1) );
            return GPARSE_OK;
        case token_literal_false:
            numeric_set( ans, _bool_(0) );
            return GPARSE_OK;
        }
    }

    status = assign_operation( ans, parser, strwct, tok_ini, tok_end );
    if (status != GPARSE_NO_COMMAND) return status;

    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, BOOLEAN_OR_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, BOOLEAN_AND_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, EQUAL_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, BITOR_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, BITXOR_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, BITAND_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, BITSHIFT_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, ARITMETIC_MASK );
    if (status != GPARSE_NO_COMMAND) return status;
    
    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, MULDIV_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = dual_operation( ans, parser, strwct, tok_ini, tok_end, POW_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = unary_operation( ans, parser, strwct, tok_ini, tok_end, ARITMETIC_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    status = unary_operation( ans, parser, strwct, tok_ini, tok_end, VARTYPE_MASK );
    if (status != GPARSE_NO_COMMAND) return status;

    /* Round Brackets */
    status = bracket( ans, parser, strwct, tok_ini, tok_end );
    if (status != GPARSE_NO_COMMAND) return status;

    parser_error( parser, "Expression error" );
    parser->code_pos = tok_ini->str_ini;
    return GPARSE_ERROR;
}

int parse_var_declaration( Parser* parser, Struct* strwct
    , const Token* _restrict_ const token_vartype
    , const Token* _restrict_ const token_name
    , const Token* _restrict_ const token_end )
{
    /* Create the variable */
    int status;
    Variable* var = strwct->add_variable
        ( token_name->str_ini, token_name->str_end, &status );

    if (status != GPARSE_NEW_NAME){
        parser_error( parser, "Variable already declared" );
        parser->code_pos = token_name->str_ini;
        return GPARSE_ERROR;
    }
    status = variable_alloc( var, token_vartype->var_type );
    if (status != GPARSE_OK){
        parser_error( parser, "C types cannot be used in declarations" );
        parser->code_pos = token_name->str_ini;
        return status;
    }

    if (token_name < token_end){
        const Token* const tok_assign = token_name + 1;
        if (tok_assign->token_type == token_assign){
            /* Assign the right side if the next token is '=' */
            Numeric right;
            int status = parse_command
                ( &right, parser, strwct, tok_assign + 1, token_end );
            if (status != GPARSE_OK) return status;

            status = variable_assign( var, &right );
            if (status != GPARSE_OK){
                parser_error( parser, "Cannot perform implicit casting" );
                parser->code_pos = tok_assign->str_ini;
                return status;
            }
            return GPARSE_OK;
        }
        else{
            parser_error( parser, "Expecting an initialization" );
            parser->code_pos = tok_assign->str_ini;
            return GPARSE_ERROR;
        }
    }
    else{
        /* Initialize the variable to the default '0' */
        memset( var->pvalue, 0, var->size );
        return GPARSE_OK;
    }
}

extern "C"
gParser* gParser_create()
{
    Parser* parser = new Parser;
    return parser;
}

extern "C"
void gParser_dispose( gParser* parser )
{
    Parser* p = (Parser*)parser;
    delete( p );
}

const Token* find_dotcomma
( const Token* _restrict_ const tok_ini
, const Token* _restrict_ const tok_last )
{
    const Token* tok_end = tok_ini;
    while (tok_end < tok_last && tok_end->token_type != token_dotcomma){
        tok_end++;
    }

    return tok_end;
}

int parser_structure
    ( Struct* str
    , const char* _restrict_ const block_ini
    , const char* _restrict_ const block_end 
    )
{
    return GPARSE_NO_COMMAND;
}

int parser_instruction_block( Parser* parser, Struct* strwct )
{
    int status;
    Numeric ans;
    const Token* tok_ini;
    const Token* tok_end;

    if (parser->num_tokens == 0){
        return GPARSE_NO_COMMAND;
    }

    tok_ini = parser->tokens;
    tok_end = parser->tokens + parser->num_tokens - 1;

    if (tok_ini < tok_end && tok_ini->token_type == token_vartype){
        /* It is a variable declaration */
        const Token* tok_name = tok_ini < tok_end ? tok_ini + 1 : nullptr;
        if (tok_name != nullptr && tok_name->token_type == token_name){
            const Token* tok_comma = tok_name;
            const Token* tok_blockend = tok_name;
            while (tok_comma <= tok_end){
                tok_name = tok_comma;
                while (tok_comma < tok_end && tok_comma->token_type != token_comma){
                    tok_comma++;
                }
                if (tok_comma->token_type == token_comma){
                    tok_blockend = tok_comma - 1;
                }
                else{
                    tok_blockend = tok_comma;
                }
                status = parse_var_declaration( parser, strwct, tok_ini, tok_name, tok_blockend );
                tok_comma++;
            }
        }
        else{
            parser_error( parser, "Expecting a variable name" );
            parser->code_pos = tok_ini->str_end;
            return GPARSE_ERROR;
        }
    }
    else if (tok_ini->token_type == token_struct){
        /* It is a structure declaration */
        const Token* tok_name = tok_ini < tok_end ? tok_ini + 1 : nullptr;
        if (tok_name != nullptr || tok_name->token_type != token_name){
            /* Add the structure name */
            Struct* str = strwct->add_struct
                ( tok_name->str_ini, tok_name->str_end, &status );
            if (status == GPARSE_NAME_COLLISION){
                parser_error( parser, "Structure name is already defined" );
                parser->code_pos = tok_ini->str_ini;
                return GPARSE_ERROR;
            }
            const Token* tok_block = tok_name < tok_end ? tok_name + 1 : nullptr;
            Parser* local_parser = new Parser();
            parser_code( local_parser, str, tok_block->str_ini + 1, tok_block->str_end - 1 );
        }
        else{
            parser_error( parser, "Expecting a structure name" );
            parser->code_pos = tok_ini->str_end;
            return GPARSE_ERROR;
        }
    }
    else if (tok_ini->token_type == token_function){
        /* It is a function declaration */

    }
    else{
        status = parse_command( &ans, parser, strwct, parser->tokens, tok_end );
        if (status == GPARSE_OK){
            variable_dynamic_assign( &parser->ans, &ans );
        }
    }

    return status;
}

int parser_code( Parser* parser, Struct* str
, const char* code_ini, const char* code_end )
{
    if (code_ini == nullptr){
        return GPARSE_NO_COMMAND;
    }

    int status = GPARSE_NO_COMMAND;

    /* Reset the parser. There are no tokens */
    parser->num_tokens = 0;

    /* Clears previous messages */
    free( parser->err_msg );
    parser->err_msg = nullptr;
    parser->code_pos = nullptr;

    /* Extract the tokens */
    const char* code_block = code_ini;
    while (*code_block != '\0'){
        if (code_end != nullptr && code_block > code_end){
            return status;
        }

        code_block = parse_tokens( parser, code_block, code_end );
        if (code_block == nullptr)
            return GPARSE_ERROR;

        status = parser_instruction_block( parser, str );

        /* Restart the parser each code block */
        parser->num_tokens = 0;

        if (status == GPARSE_ERROR){
            /* There is a syntactic error */
            if (parser->code_pos != nullptr){
                parser->err_column = parser->code_pos - code_ini;
            }
            return GPARSE_ERROR;
        }
    }

    return status;
}

extern "C"
int gParser_command( gParser* gparser, const char* code )
{
    Parser* parser = (Parser*)gparser;
    int status;

    if (code == nullptr){
        return GPARSE_NO_COMMAND;
    }
    status = parser_code( parser, &parser->global, code, nullptr );

    return status;
}

