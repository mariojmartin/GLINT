/***
Copyright (c) 2016 Mario J. Martin-Burgos <dominonurbs$gmail.com>
This softaware is licensed under Apache 2.0 license
http://www.apache.org/licenses/LICENSE-2.0

*******************************************************************************/

#ifndef H_GPARSER_H
#define H_GPARSER_H

#include <stdlib.h>
#include <memory.h>

#include "gdata.h"
#include "data_wrap.hpp"

static TokenType identify_keyword
    ( int* vartype
    , const char* const init
    , const char* const end 
    )
{
    const char* p = init;

    if (strtok_compare( "bool", p, end ) == 0){
        *vartype = t_bool;
        return token_vartype;
    }
    else if (strtok_compare( "byte", p, end ) == 0){
        *vartype = t_byte;
        return token_vartype;
    }
    else if (strtok_compare( "int", p, end ) == 0){
        *vartype = t_int;
        return token_vartype;
    }
    else if (strtok_compare( "int64", p, end ) == 0){
        *vartype = t_l64;
        return token_vartype;
    }
    else if (strtok_compare( "float", p, end ) == 0){
        *vartype = t_float;
        return token_vartype;
    }
    else if (strtok_compare( "double", p, end ) == 0){
        *vartype = t_double;
        return token_vartype;
    }
    else if (strtok_compare( "and", p, end ) == 0){
        *vartype = 0;
        return token_and;
    }
    else if (strtok_compare( "or", p, end ) == 0){
        *vartype = 0;
        return token_or;
    }
    else if (strtok_compare( "xor", p, end ) == 0){
        *vartype = 0;
        return token_bitxor;
    }
    else if (strtok_compare( "true", p, end ) == 0){
        *vartype = 0;
        return token_literal_true;
    }
    else if (strtok_compare( "false", p, end ) == 0){
        *vartype = 0;
        return token_literal_false;
    }
    else if (strtok_compare( "void", p, end ) == 0){
        *vartype = 0;
        return token_void;
    }
    else if (strtok_compare( "struct", p, end ) == 0){
        *vartype = 0;
        return token_struct;
    }
    else if (strtok_compare( "function", p, end ) == 0){
        *vartype = 0;
        return token_struct;
    }
    else{
        return token_null;
    }
}

static inline void parser_push_name
( Parser* parser
, const char* _restrict_ const p0, const char* _restrict_ const p1
)
{
    if (p0 != p1){
        int var_type = 0;
        TokenType token_type = identify_keyword( &var_type, p0, p1 );
        if (token_type != token_null){
            /* Add a known type */
            parser->add_token( p0, p1, token_type, var_type );
        }
        else{
            if (*p0 >= '0' && *p0 <= '9'){
                /* It should be a literal */
                parser->add_token( p0, p1, token_literal_number );
            }
            else{
                /* Add a name, which can be a variable or a explicit constant */
                parser->add_token( p0, p1, token_name );
            }
        }
    }
}

static inline void parser_push_1
( Parser* parser
, const char** _restrict_ const p0, const char** _restrict_ const p1
, const TokenType token_type
)
{
    parser_push_name( parser, *p0, *p1 );
    *p0 = *p1 + 1;
    parser->add_token( *p1, *p0, token_type );
}

static inline void parser_push_2
( Parser* parser
, const char** _restrict_ const p0, const char** _restrict_ const p1
, const TokenType token_type
)
{
    parser_push_name( parser, *p0, *p1 );
    *p0 = *p1 + 2;
    parser->add_token( *p1, *p0, token_type );
    (*p1)++;
}

static inline void parser_push_3
( Parser* parser
, const char** _restrict_ const p0, const char** _restrict_ const p1
, const TokenType token_type
)
{
    parser_push_name( parser, *p0, *p1 );
    *p0 = *p1 + 3;
    parser->add_token( *p1, *p0, token_type );
    (*p1)++;
}

const char* wipe_comment( const char* p )
{
    while (*p != '*' || *(p + 1) != '/'){
        if (*p == '\0'){
            return nullptr;
        }
        p++;
    }
    return p + 2;
}

const char* wipe_line_comment( const char* p )
{
    while (*p != '\0'){
        if (*p == '\n'){
            return p + 1;
        }
        p++;
    }
    return p;
}

const char* find_closing_bracket( const char* p )
{
    while (*p != '}'){
        if (*p == '\0'){
            return nullptr;
        }
        p++;
    }
    return p;
}

static const char* parse_tokens
( Parser* parser, const char* const code_ini, const char* const code_end )
{
    register const char* _restrict_ p0 = code_ini;
    register const char* _restrict_ p = p0;

    /* Clean command separators */
    while (*p0 == ';' || *p0 == '\n'){
        p0++;
        p = p0;
    }

    /* Get the left token and the token operand */
    while (1){
        /* Clean white spaces */
        while (*p0 == ' ' || *p0 == '\t' || *p0 == '\r'){
            p0++;
            p = p0;
        }

        if (*p0 == '\\'){
            p0++;
            p = p0;
            /* Ignore the next newline */
            while (*p0 == ' ' || *p0 == '\t' || *p0 == '\r' || *p0 == '\n'){
                p0++;
            }
            p = p0;
        }

        const char next = *(p + 1);

        if (code_end != nullptr && p0 > code_end){
            parser_push_name( parser, p0, p );
            return p;
        }

        switch (*p){
        case '\0':
            parser_push_name( parser, p0, p );
            return p;

        case '\n':
            parser_push_name( parser, p0, p );
            return p;

        case ';':
            parser_push_name( parser, p0, p );
            return p;

        case ' ':
            parser_push_name( parser, p0, p );
            p0 = p;
            continue;

        case '\t':
            parser_push_name( parser, p0, p );
            p0 = p;
            continue;

        case '\r':
            parser_push_name( parser, p0, p );
            p0 = p;
            continue;

        case ',':
            parser_push_1( parser, &p0, &p, token_comma );
            break;

        case '{':{
            /* Ignore everything until it finds the closing bracket */
            parser_push_name( parser, p0, p );
            const char* pb = find_closing_bracket( p + 1 );
            if (pb == nullptr){
                parser_error( parser, "Unclosed bracket" );
                parser->code_pos = p0;
                return nullptr;
            }
            parser->add_token( p, pb, token_brackets );
            p = pb + 1;
            p0 = p;
            continue;
        }
        case '}':
            parser_error( parser, "Unexpected closing bracket" );
            return nullptr;

        case '(':
            parser_push_1( parser, &p0, &p, token_bracket_round_open );
            break;

        case ')':
            parser_push_1( parser, &p0, &p, token_bracket_round_close );
            break;

        case '+':
            switch (next){
            case '+':
                parser_push_2( parser, &p0, &p, token_plusplus );
                break;
            case '=':
                parser_push_2( parser, &p0, &p, token_add_assign );
                break;
            default:
                /* Avoid the plus in the notation 1e+1 */
                if (!(p > p0 && (*(p - 1) == 'e' || *(p - 1) == 'E')))
                    parser_push_1( parser, &p0, &p, token_plus );
            }
            break;

        case '-':
            switch (next){
            case '-':
                parser_push_2( parser, &p0, &p, token_minusminus );
                break;
            case '=':
                parser_push_2( parser, &p0, &p, token_minus_assign );
                break;
            default:
                /* Avoid the plus in the notation 1e+1 */
                if (!(p > p0 && (*(p - 1) == 'e' || *(p - 1) == 'E')))
                    parser_push_1( parser, &p0, &p, token_minus );
            }
            break;

        case '*':
            switch (next){
            //case '*':
            //    parser_push_2( parser, &p0, &p, token_pow );
            //    break;
            case '=':
                parser_push_2( parser, &p0, &p, token_mul_assign );
                break;
            case '/':
                parser->code_pos = p;
                parser_error( parser, "Unexpected closing comment" );
                return nullptr;
            default:
                parser_push_1( parser, &p0, &p, token_mul );
            }
            break;

        case '/':
            switch (next){
            case '/':{
                parser_push_name( parser, p0, p );
                p = wipe_line_comment( p );
                p0 = p;
                continue;
            }
            case '*':{
                parser_push_name( parser, p0, p );
                const char* pend = wipe_comment( p );
                if (pend == nullptr){
                    parser->code_pos = p;
                    parser_error( parser, "Unclosed comment" );
                    return nullptr;
                }
                p = pend;
                p0 = p;
                continue;
            }
            case '=':
                parser_push_2( parser, &p0, &p, token_div_assign );
            default:
                parser_push_1( parser, &p0, &p, token_div );
            }
            break;

        case '%':
            switch (next){
            case '%':
                parser_push_2( parser, &p0, &p, token_intdiv );
                break;
            default:
                parser_push_1( parser, &p0, &p, token_remainder );
            }
            break;

        case '=':
            if (next == '=')
                parser_push_2( parser, &p0, &p, token_equal );
            else
                parser_push_1( parser, &p0, &p, token_assign );
            break;

        case '&':
            switch (next){
            case '&':
                parser_push_2( parser, &p0, &p, token_and );
                break;
            case '=':
                parser_push_2( parser, &p0, &p, token_bitand_assign );
                break;
            default:
                parser_push_1( parser, &p0, &p, token_bitand );
            }
            break;

        case '|':
            switch (next){
            case '|':
                parser_push_2( parser, &p0, &p, token_or );
                break;
            case '=':
                parser_push_2( parser, &p0, &p, token_bitor_assign );
                break;
            default:
                parser_push_1( parser, &p0, &p, token_bitor );
            }
            break;

        case '!':
            if (next == '=')
                parser_push_2( parser, &p0, &p, token_notequal );
            else
                parser_push_1( parser, &p0, &p, token_not );
            break;

        case '>':
            switch (next){
            case '=':
                parser_push_2( parser, &p0, &p, token_greaterequal );
                break;
            case '>':
                parser_push_2( parser, &p0, &p, token_rshift );
                break;
            default:
                parser_push_1( parser, &p0, &p, token_greater );
            }
            break;

        case '<':
            switch (next){
            case '=':
                parser_push_2( parser, &p0, &p, token_lessequal );
                break;
            case '<':
                parser_push_2( parser, &p0, &p, token_lshift );
                break;
            default:
                parser_push_1( parser, &p0, &p, token_less );
            }
            break;

        case '^':
            if (next == '=')
                parser_push_2( parser, &p0, &p, token_bitxor_assign );
            else
                parser_push_1( parser, &p0, &p, token_bitxor );
            break;

        } /* end switch */

        p++;
    } /* end while */

    return p;
}


#endif /* H_GPARSER_H */
