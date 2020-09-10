#ifndef CONPETITOR_H
#define COMPETITOR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
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
    uint8_t scores[6];
};

/** make for the easier store */
struct list {
    struct sim_comp competitor;
    struct sim_comp* next;
};

extern __cdecl struct sim_comp get_competitor();
extern __cdecl int read_competitors(struct sim_comp** out_comps_array);

#if !defined(AUTO_APPEND) && defined(REF_APPEND)
#define append_competitor(ref_comp) _append_competitor_ref(ref_comp)
#elif !defined(REF_APPEND) && defined(AUTO_APPEND)
#define append_competitor _append_competitor_none
#else
#define append_competitor _append_competitor_none
#endif

extern __cdecl bool adjust_name(struct sim_comp* ref_comp);
extern __cdecl bool adjust_class(struct sim_comp* ref_comp);
extern __cdecl bool adjust_competitors_information(struct sim_comp* ref_comp);
extern __cdecl bool adjust_scores(struct sim_comp* ref_comp);
extern __cdecl void* _adjust_score_sig(struct sim_comp* ref_comp,
                                       void (*srand)(unsigned int));

#endif