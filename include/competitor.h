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

#ifdef SFILE
#undef SFILE
#endif
#define SFILE "./comptetors"

/** redefination of competitor*/
typedef struct sim_comp competitors;

/** real defination of competitor */
struct sim_comp {
    int id;
    char name[16];
    char _class[32];
    char topic_name[64];
    char scores[6];
};

/** make for the easier store */
struct list {
    struct sim_comp competitor;
    struct sim_comp* next;
};

extern struct sim_comp get_competitor();
extern int read_competitors(struct sim_comp** out_comps_array);

#endif