/*
Copyright (c) 2016 Mario J. Martin-Burgos <dominonurbs$gmail.com>
This softaware is licensed under Apache 2.0 license
http://www.apache.org/licenses/LICENSE-2.0

*******************************************************************************/

#ifndef H_GDEFINES_H
#define H_GDEFINES_H

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "gdata.h"

#define NAME_MASK (1<<4)
#define ARITMETIC_MASK (1<<5)
#define MULDIV_MASK (1<<6)
#define POW_MASK (1<<7)
#define EQUAL_MASK (1<<8)
#define BOOLEAN_OR_MASK (1<<9)
#define BOOLEAN_AND_MASK (1<<10)
#define BITAND_MASK (1<<11)
#define BITXOR_MASK (1<<12)
#define BITOR_MASK (1<<13)
#define BITSHIFT_MASK (1<<14)
#define ASSIGN_MASK (1<<15)
#define POSTFIT_MASK (1<<16)
#define BRACKETS_MASK (1<<17)
#define COMMENTS_MASK (1<<18)
#define UNARY_MASK  (1<<19)
#define OTHER_MASK  (1<<20)
#define KEYWORD_MASK  (1<<21)
#define VARTYPE_MASK  (1<<22)

enum TokenType : unsigned int
{
    token_null = 0,
    token_name = NAME_MASK,         // A variable or function
    token_literal_number = NAME_MASK + 1,  // 1, 1.5f, ...
    token_literal_true = NAME_MASK + 2,    // true
    token_literal_false = NAME_MASK + 3,   // false
    token_vartype = VARTYPE_MASK,   // bool, int, double, ...

    /* Unary */
    token_bitinv = UNARY_MASK + 1,   // ~
    token_not = UNARY_MASK + 2,      // !

    /* Separators */
    token_comma = OTHER_MASK + 1,    // ,
    token_dotcomma = OTHER_MASK + 2, // ;
    token_newline = OTHER_MASK + 3,  // \n

    /* Keywords */
    token_void = KEYWORD_MASK + 1,     // void
    token_struct = KEYWORD_MASK + 2,   // struct
    token_function = KEYWORD_MASK + 3, // function

    /* Arithmetic */
    token_plus = ARITMETIC_MASK + 1,    // +
    token_minus = ARITMETIC_MASK + 2,   // -

    token_mul = MULDIV_MASK + 1,        // *
    token_div = MULDIV_MASK + 2,        // /

    token_remainder = MULDIV_MASK + 3,  // %
    token_intdiv = MULDIV_MASK + 4,     // /%

    token_pow = POW_MASK,         // Not defined

    /* Bitwise */
    token_bitand = BITAND_MASK + 1,    // &
    token_bitxor = BITXOR_MASK + 2,    // xor
    token_bitor = BITOR_MASK + 3,      // |
    token_lshift = BITSHIFT_MASK + 4,  // <<
    token_rshift = BITSHIFT_MASK + 5,  // >>

    /* Boolean */
    token_equal = EQUAL_MASK + 1,        // ==
    token_notequal = EQUAL_MASK + 2,     // !=
    token_less = EQUAL_MASK + 3,         // <
    token_greater = EQUAL_MASK + 4,      // >
    token_lessequal = EQUAL_MASK + 5,    // <=
    token_greaterequal = EQUAL_MASK + 6, // >=

    token_and = BOOLEAN_AND_MASK + 1,   // &&
    token_or = BOOLEAN_OR_MASK + 2,     // ||

    /* Assign */
    token_assign = ASSIGN_MASK + 1,         // =
    token_add_assign = ASSIGN_MASK + 2,     // +=
    token_minus_assign = ASSIGN_MASK + 3,   // -=
    token_mul_assign = ASSIGN_MASK + 4,     // *=
    token_div_assign = ASSIGN_MASK + 5,     // /=
    token_remainder_assign = ASSIGN_MASK + 5,   // %=
    token_intdiv_assign = ASSIGN_MASK + 5,      // /%=
    token_bitand_assign = ASSIGN_MASK + 6,      // &=
    token_bitor_assign = ASSIGN_MASK + 7,       // |=
    token_bitxor_assign = ASSIGN_MASK + 8,         // ^=
    token_bitor_lshiftassign = ASSIGN_MASK + 9, // <<=
    token_bitor_rshiftassign = ASSIGN_MASK + 10,// >>=

    /* Posit */
    token_plusplus = POSTFIT_MASK + 1,       // ++
    token_minusminus = POSTFIT_MASK + 2,     // --

    /* Brackets */
    token_bracket_round_open = BRACKETS_MASK + 1,   // (
    token_bracket_round_close = BRACKETS_MASK + 2,  // )
    token_brackets = BRACKETS_MASK + 3,   // (

    /* Comments */
    token_comment_line = COMMENTS_MASK + 1,  // //
    token_comment_open = COMMENTS_MASK + 2,  // /*
    token_comment_close = COMMENTS_MASK + 3, // */
};

/* Much like strcmp() */
static inline int strtok_compare
( const char* _restrict_ const name
, const char* _restrict_ const ini
, const char* _restrict_ const end
)
{
    const char* a = name;
    const char* b = ini;

    while (b < end){
        if (*b > *a){
            return 1;
        }
        else if (*b < *a || *a == '\0'){
            return -1;
        }
        a++;
        b++;
    }

    if (*a == '\0'){
        return 0;
    }
    else{
        return 1;
    }
}

char* tok2str
( const char* _restrict_ const ini
, const char* _restrict_ const end
)
{
    char* varname;
    const char* p = ini;
    int size = 0;

    /* Calculate the number of characters */
    while (p < end && *p != '\0' && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r'){
        size++;
        p++;
    }

    /* Copy the name */
    varname = (char*)_malloc_( sizeof( char ) * (size + 1) );
    if (varname != nullptr){
        p = ini;
        char* d = varname;
        for (int i = 0; i < size; i++){
            *d = *p;
            p++;
            d++;
        }
        *d = '\0';
    }

    return varname;
}

template< class T >
struct BinaryTree : T
{
    BinaryTree< T >* upper_name;
    BinaryTree< T >* lower_name;

    BinaryTree( char* name ) : T( name )
    {
        upper_name = nullptr;
        lower_name = nullptr;
    }

    /* Adds a new variable with the specified type and name.
    * If there is a variable with the same name, it returns nullptr */
    T* push( const char* _restrict_ const ini
        , const char* _restrict_ const end, int* status )
    {
        int cmp = strtok_compare( this->name, ini, end );
        if (cmp == 0){
            /* There is already a variable with the same name */
            *status = GPARSE_NAME_COLLISION;
            return this;
        }
        else if (cmp > 0){
            if (upper_name == nullptr){
                char* varname = tok2str( ini, end );
                upper_name = new BinaryTree< T >( varname );
                *status = GPARSE_NEW_NAME;
                return upper_name;
            }
            else{
                return upper_name->push( ini, end, status );
            }
        }
        else{
            if (lower_name == nullptr){
                char* varname = tok2str( ini, end );
                lower_name = new BinaryTree< T >( varname );
                *status = GPARSE_NEW_NAME;
                return lower_name;
            }
            else{
                return lower_name->push( ini, end, status );
            }
        }
    }

    /* Gets the variable with the specified name.
    * If there is not such variable, it returns nullptr */
    T* find
        ( const char* _restrict_ const ini, const char* _restrict_ const end )
    {
        int cmp = strtok_compare( this->name, ini, end );
        if (cmp == 0){
            /* We found the variable */
            return this;
        }
        else if (cmp > 0){
            if (upper_name != nullptr){
                return upper_name->find( ini, end );
            }
            else{
                return nullptr;
            }
        }
        else{
            if (lower_name != nullptr){
                return lower_name->find( ini, end );
            }
            else{
                return nullptr;
            }
        }
    }

    void dispose()
    {
        if (upper_name != nullptr){
            upper_name->dispose();
            delete upper_name;
        }
        if (lower_name != nullptr){
            lower_name->dispose();
            delete lower_name;
        }
    }
};

struct Variable : gVariable
{
    Variable( char* _varname )
    {
        memset( this, 0, sizeof( Variable ) );
        this->name = _varname;
    }

    ~Variable(){
        free( name );
        free( pvalue );
    }
};

struct Struct
{
    char* name;
    int nvars;
    BinaryTree< Variable >* vars;
    BinaryTree< Struct >* strs;

    /* Data structures are stored in a binary tree for quick search */
    Struct* upper_name;
    Struct* lower_name;

    Struct(){
        memset( this, 0, sizeof( Struct ) );
    }

    Struct( char* _name ){
        memset( this, 0, sizeof( Struct ) );
        name = _name;
    }

    ~Struct(){
        dispose();
    }

    void dispose()
    {
        free( name );

        if (vars != nullptr){
            vars->dispose();
            delete vars;
            vars = nullptr;
        }

        /* Delete structures */
        if (strs != nullptr){
            strs->dispose();
            delete strs;
            strs = nullptr;
        }
    }

    /* Returns the variable.
    * If status == 0, the variable is new,
    * if status == 1 the variable already exits*/
    Variable* add_variable( const char* _restrict_ const ini
        , const char* _restrict_ const end, int* status )
    {
        if (vars != nullptr){
            return vars->push( ini, end, status );
        }
        else{
            char* varname = tok2str( ini, end );
            vars = new BinaryTree< Variable >( varname );
            *status = GPARSE_NEW_NAME;
            return vars;
        }
    }

    Variable* find_variable
        ( const char* _restrict_ const ini, const char* _restrict_ const end )
    {
        Variable* obj;
        if (vars != nullptr){
            obj = vars->find( ini, end );
        }
        else{
            obj = nullptr;
        }

        return obj;
    }

    /* Returns the variable.
    * If status == 0, the struct is new,
    * if status == 1 a There is already a struct with the same name */
    Struct* add_struct( const char* _restrict_ const ini
        , const char* _restrict_ const end, int* status )
    {
        if (strs != nullptr){
            return strs->push( ini, end, status );
        }
        else{
            char* name = tok2str( ini, end );
            strs = new BinaryTree< Struct >( name );
            *status = GPARSE_NEW_NAME;
            return strs;
        }
    }

    Struct* find_struct
        ( const char* _restrict_ const ini, const char* _restrict_ const end )
    {
        Struct* obj;
        if (strs != nullptr){
            obj = strs->find( ini, end );
        }
        else{
            obj = nullptr;
        }

        return obj;
    }
};

struct Token
{
    const char* str_ini;  /* Pointer to the string at the start of the token */
    const char* str_end;  /* Pointer to the string at the end of the token */
    TokenType token_type; /* Indicates if it is an operand, a var name, ... */
    int var_type;         /* If the token is a variable, indicates the type.
                          * Basic types are negative, while positive types
                          * are the index as registered in the parser. */
};

struct Parser : gParser
{
    Struct global;

    /* Indicates the position of the erro or warning in the string */
    const char* code_pos;

    /* Probably, it will never be more than 256 tokens in a command :/ */
    Token tokens[256];
    size_t num_tokens;

    Parser()
    {
        memset( this, 0, sizeof( Parser ) );
    }

    ~Parser()
    {
        /* Erase global variables and structure, function definitions */
        global.dispose();

        /* Delete output messages */
        free( this->err_msg );

        /* Release the 'ans' varable */
        free( ans.pvalue );
        free( ans.name );
    }

    void add_token
        ( const char* _restrict_ const ini
        , const char* _restrict_ const end
        , const TokenType token_type
        )
    {
        if (ini < end){
            Token* ptok = tokens + num_tokens;
            ptok->str_ini = ini;
            ptok->str_end = end;
            ptok->token_type = token_type;
            ptok->var_type = 0;
            num_tokens++;
        }
    }

    void add_token
        ( const char* _restrict_ const ini
        , const char* _restrict_ const end
        , const TokenType token_type
        , const int var_type
        )
    {
        if (ini < end){
            Token* ptok = tokens + num_tokens;
            ptok->str_ini = ini;
            ptok->str_end = end;
            ptok->token_type = token_type;
            ptok->var_type = var_type;
            num_tokens++;
        }
    }
};

/* Creates an error message */
void parser_error
( Parser* parser, const char* _restrict_ string, ... )
{
    va_list argptr;
    va_start( argptr, string );

    if (string != nullptr){
        char buffer[1024];
        vsprintf( buffer, string, argptr );
        size_t msg_len = strlen( buffer );
        free( parser->err_msg );
        parser->err_msg = (char*)malloc( sizeof( char )*(msg_len + 1) );
        if (parser->err_msg != nullptr){
            memcpy( parser->err_msg, buffer, sizeof( char )*(msg_len + 1) );
        }
    }
}

#endif /* H_GDEFINES_H */

