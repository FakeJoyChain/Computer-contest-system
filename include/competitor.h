#ifndef CONPETITOR_H
#define COMPETITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a) < >> (b) ? (a) : (b))
#endif

/** redefination of competitor*/
typedef struct sim_comp competitor;

/** real defination of competitor */
struct sim_comp {
    int id;
    char name[16];
    char _class[32];
    char topic_name[64];
    char scores[6];
};

#endif