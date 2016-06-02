#include "gparser.h"

extern "C"
double gVariable_getasDouble( const gVariable var )
{
    switch (var.type){
        case t_undefined:{  /* The variable is not set */
            const unsigned u = ~0;
            return *((double*)&u);
            break;
        }
        case t_bool:{       /* Boolean type */
            char* p = (char*)var.pvalue;
            if (*p){
                return 1;
            }
            else{
                return 0;
            }
            break;
        }
        case t_int:{        /* Equivalent to 'int' in C */
            int* p = (int*)var.pvalue;
            return (double)(*p);
            break;
        }
        case t_double:{     /* Equivalent to 'double' in C */
            double* p = (double*)var.pvalue;
            return *p;
            break;
        }
        case t_l64:{        /* integer of 64 bits */
            int64_t* p = (int64_t*)var.pvalue;
            return (double)(*p);
            break;
        }
        case t_float:{      /* Equivalent to 'float' in C */
            float* p = (float*)var.pvalue;
            return (double)(*p);
            break;
        }

        default:
            const unsigned u = ~0;
            return *((double*)&u);
    }
}
