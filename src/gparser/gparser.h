/** @file

Copyright (c) 2016 Mario J. Martin-Burgos <dominonurbs$gmail.com>
This softaware is licensed under Apache 2.0 license
http://www.apache.org/licenses/LICENSE-2.0

*******************************************************************************/

#ifndef H_GPARSER_H
#define H_GPARSER_H

#include "gdata.h"

#if defined(__cplusplus)
extern "C" {
#endif
    /** 
    Creates a new parser */
    gParser* gParser_create();

    /** Releases memory resources */
    void gParser_dispose( gParser* parser );

    /** 
    Executes the arithmetic operations in the string.
    @param parser Pointer to the parser object. The result of the arithmetic 
    operations is stored in parser->ans
    @param code String with the operations to be parsed
    @return 
        - GPARSE_OK if the command is succesfully parsed
        - GPARSE_ERROR if there is a syntactic error 
        - GPARSE_NO_COMMAND if the string is empty
     */
    int gParser_command( gParser* parser, const char* code );

    /** Cast a variable into double. */
    double gVariable_getasDouble( const gVariable var );
#if defined(__cplusplus)
}
#endif

#endif /* H_GPARSER_H */
