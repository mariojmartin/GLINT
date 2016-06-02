/***
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
    gParser* gParser_create();
    void gParser_dispose( gParser* parser );
    int gParser_command( gParser* gparser, const char* code );
    double gVariable_getasDouble( const gVariable var );
#if defined(__cplusplus)
}
#endif

#endif /* H_GPARSER_H */
