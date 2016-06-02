/***
Author: Mario J. Martin <dominonurbs$gmail.com>

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
